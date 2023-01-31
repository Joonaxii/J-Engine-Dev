#include <JEngine/Rendering/Renderer.h>
#include <JEngine/Rendering/RenderingConsts.h>
#include <JEngine/Rendering/Window.h>
#include <JEngine/Utility/JTime.h>
#include <JEngine/Assets/Graphics/Shader/Shader.h>
#include <JEngine/Assets/Decoders/BMPDecoder.h>
#include <JEngine/Assets/Graphics/Texture/Texture2D.h>
#include <JEngine/Assets/Graphics/Sprite.h>
#include <JEngine/Debugging/FPS.h>
#include <JEngine/Math/Math.h>
#include <JEngine/Input/Input.h>
#include <imgui.h>
#include "../../Game/include/SimpleRenderer.h"
#include <JEngine/Rendering/ICamera.h>
#include <JEngine/Rendering/ImGui/IImGuiDrawable.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <fstream>
#include <nlohmann/json.hpp>

static const std::string SHADER_PATH = "../../../J-Engine-Editor/Assets/builtin/shaders/";
static const std::string TEXTURE_PATH = "../../../Game/Assets/textures/";

using namespace JEngine;

struct DebugCamera : public ICamera {
public:
    JMatrix4f matrix;

    DebugCamera() : matrix({ 0, 0 }, 0, {1, 1}) {
        _clearColor = JColor32::Black;
    }

    void setTransformMatrix(const JMatrix4f& mat) override {
        matrix = mat;
    }

    const JMatrix4f& getTransformMatrix() const override {
        return matrix;
    }

protected:
    virtual const bool jsonToBinaryImpl(json& jsonF, std::ostream& stream) const override { return false; }
};

struct DebugWindow : public IImGuiDrawable {
    struct TransformSettings {
        JVector2f pos = {0, 0};
        float rot = 0;
        JVector2f size = {1.0f, 1.0f};
    };

    struct CameraSettings {
        JColor clearColor = JColor::Black;
        JColor tintColor = JColor::White;

        JRectf viewRect = JRectf(0, 0, 1, 1);
        JRectf screenRect = JRectf(0, 0, 1, 1);

        TransformSettings transform;
        bool isActive;
    };

    Renderer& renderer;
    JTime& timeRef;
    FPS& fps;

    bool debugOpen;

    float rumbles[8]{ 0 };
    float temp[2]{ 0 };
    bool keepRumble[4]{ false };

    std::vector<ICamera*> cameras;
    std::vector<CameraSettings> camSettings;

    std::vector<SimpleRenderer*> simpleRenderers;
    std::vector<TransformSettings> rendTransforms;

    DebugWindow(Renderer& renderer, JTime& timeRef, FPS& fpsRef, ICamera** cams, size_t camCount, SimpleRenderer* renderers, size_t rendCount) :
        debugOpen(true), timeRef(timeRef), fps(fpsRef), renderer(renderer)
        { 
        cameras.clear();
        camSettings.clear();

        simpleRenderers.clear();
        rendTransforms.clear();

        for (size_t i = 0; i < camCount; i++) {
            auto cam = cams[i];
            CameraSettings set;

            if (cam) {
                set.clearColor = cam->getClearColor();
                set.tintColor = cam->getCameraTint();
                set.isActive = !cam->isCameraDisabled();

                cam->getTransformMatrix().decompose(set.transform.pos, set.transform.rot, set.transform.size);
            }

            cameras.push_back(cam);
            camSettings.push_back(set);
        }

        for (size_t i = 0; i < rendCount; i++) {
            auto rend = &renderers[i];
            TransformSettings set;

            simpleRenderers.push_back(rend);
            rendTransforms.push_back(set);
        }
    }

    void onImGui() override {

        auto& rendInf = renderer.getRenderInfo();
        char buffer[64]{ '\0' };
        //ImGui::Begin("")

        if (Input::isDown(Input::DEV_Keyboard, Input::INP_F1)) {
            debugOpen = !debugOpen;
        }

        if (debugOpen) {
            if (ImGui::Begin("Debug Info", &debugOpen)) {
                if (ImGui::CollapsingHeader("Frame Info", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
                    ImGui::Text("FPS: %f", fps.getFPS());
                    ImGui::Text("Time: %f", timeRef.getEngineTime());
                    ImGui::Text("Delta: %f", timeRef.getEngineDeltaTime());
                    ImGui::Text("Frames: %d", timeRef.getFrameCount());
                }

                if (ImGui::CollapsingHeader("Render Info", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
                    ImGui::Text("Active Renderers: %i", rendInf.activeRenderers);
                    ImGui::Text("Draw Calls: %i", rendInf.drawCalls);
                    ImGui::Text("Vertex Count: %i", rendInf.vertexCount);
                    ImGui::Text("Triangle Count: %i", rendInf.triCount);
                }

                
                if (ImGui::CollapsingHeader("Cameras")) {

                    for (size_t i = 0; i < cameras.size(); i++)
                    {
                        ImGui::PushID(i);
                        memset(buffer, 0, sizeof(buffer));
                        sprintf_s(buffer, "Camera #%i", i);
                        if (ImGui::CollapsingHeader(buffer)) {
                            auto camera = cameras[i];
                            auto& cameraSet = camSettings[i];
                            bool changedTr = false;
                            bool changedColor = false;
                            bool changedView = false;

                            ImGui::BeginDisabled(!camera);

                            TransformSettings& tr = cameraSet.transform;

                            if (ImGui::Checkbox("Is Active##Cam", &cameraSet.isActive)) {
                                camera->setCameraDisabled(!cameraSet.isActive);
                            }

                            changedTr |= ImGui::DragFloat2("Position##Cam", reinterpret_cast<float*>(&tr.pos), 0.1f);
                            changedTr |= ImGui::DragFloat("Rotation##Cam", &tr.rot, 0.1f);
                            changedTr |= ImGui::DragFloat2("Size##Cam", reinterpret_cast<float*>(&tr.size), 0.1f);

                            changedView |= ImGui::DragFloat4("View Rect", reinterpret_cast<float*>(&cameraSet.viewRect), 0.1f, 0.0f, 1.0f);
                            changedView |= ImGui::DragFloat4("Screen Rect", reinterpret_cast<float*>(&cameraSet.screenRect), 0.1f, 0.0f, 0);

                            changedColor |= ImGui::ColorEdit4("Clear Color", reinterpret_cast<float*>(&cameraSet.clearColor));
                            changedColor |= ImGui::ColorEdit4("Tint Color", reinterpret_cast<float*>(&cameraSet.tintColor));

                            ImGui::EndDisabled();

                            if (camera) {
                                if (changedTr) {
                                    camera->setTransformMatrix(JMatrix4f(cameraSet.transform.pos, cameraSet.transform.rot, cameraSet.transform.size));
                                }

                                if (changedView) {
                                    camera->setViewRect(cameraSet.viewRect);
                                    camera->setScreenRect(cameraSet.screenRect);
                                }

                                if (changedColor) {
                                    camera->setClearColor(cameraSet.clearColor);
                                    camera->setCameraTint(cameraSet.tintColor);
                                }
                            }
                        }
                        ImGui::PopID();
                    }
                }

                if (ImGui::CollapsingHeader("Objects")) {

                    for (size_t i = 0; i < simpleRenderers.size(); i++)
                    {
                        ImGui::PushID(i);
                        memset(buffer, 0, sizeof(buffer));
                        sprintf_s(buffer, "Object #%i", i);
                        if (ImGui::CollapsingHeader(buffer)) {
                            auto object = simpleRenderers[i];
                            auto& objTr = rendTransforms[i];
                            bool changedTr = false;
                   
                            ImGui::BeginDisabled(!object);

                            changedTr |= ImGui::DragFloat2("Position##Obj", reinterpret_cast<float*>(&objTr.pos), 0.1f);
                            changedTr |= ImGui::DragFloat("Rotation##Obj", &objTr.rot, 0.1f);
                            changedTr |= ImGui::DragFloat2("Size##Obj", reinterpret_cast<float*>(&objTr.size), 0.1f);

                            ImGui::EndDisabled();

                            if (object) {
                                if (changedTr) {
                                    object->setTRS(objTr.pos, objTr.rot, objTr.size);
                                }
                            }
                        }
                        ImGui::PopID();
                    }
                }

                if (ImGui::CollapsingHeader("Controller Info", ImGuiTreeNodeFlags_Framed)) {
                    Input::Gamepad pads[4]{};
                    Input::getGamepads(Input::DEV_One | Input::DEV_Two | Input::DEV_Three | Input::DEV_Four, pads);
                    char buffer[24]{ '\0' };

                    auto rumbl = rumbles;
                    for (int32_t i = 0; i < 4; i++, rumbl += 2)
                    {
                        const int len = sprintf_s(buffer, "Controller #%i", i);
                        buffer[len + 1] = '\0';

                        ImGui::PushID(i);
                        if (ImGui::CollapsingHeader(buffer, ImGuiTreeNodeFlags_Framed)) {
                            const bool connected = pads[i].isConnected();
                            ImGui::Text("Is Connected: %s", (connected ? "Yes!" : "Nah!"));

                            memcpy(temp, rumbl, sizeof(temp));
                            ImGui::SliderFloat2("Rumble Amount", rumbl, 0.0f, 100.0f, "%.3f");

                            bool changed = *reinterpret_cast<uint64_t*>(temp) != *reinterpret_cast<uint64_t*>(rumbl);
                            const bool prev = keepRumble[i];

                            ImGui::BeginDisabled(prev || !connected);
                            const bool press = ImGui::Button("Set Rumble");
                            ImGui::EndDisabled();

                            ImGui::SameLine();
                            if (ImGui::Checkbox("Auto Update", keepRumble + i) && connected && keepRumble[i]) {
                                changed = true;
                            }

                            const bool keep = keepRumble[i];
                            changed |= prev != keep;

                            if (press || (keep && changed)) {
                                Input::vibrateDevice(i, rumbl[0] * 0.01f, rumbl[1] * 0.01f);
                            }
                        }
                        ImGui::PopID();
                    }
                }
            }
            ImGui::End();
        }
    }


private:

};

int main() {

    Renderer renderer;    
    if (!renderer.init("J-Engine [Editor]")) {
        renderer.terminate();
        return -1;
    }

    {
        Shader shader(SHADER_PATH + "Textured.shader");
        Shader shaderBlend(SHADER_PATH + "FB-Default-Blend.shader");
        auto dec = decodeBmp(TEXTURE_PATH + "Simba.bmp");
        Texture2D texA;

        if (dec.errFlags == DECODER_OK) {
            texA.create(dec.resolution.x, dec.resolution.y, dec.format, FilterMode::Nearest, dec.data, false);
        }
        else {
            std::string err;
            std::cout << "Failed to read texture! [" << getBmpErrorString(err, dec.errFlags) << "]\n";
        }
        dec.clear(false);

        dec = decodeBmp(TEXTURE_PATH + "Blade.bmp");
        Texture2D texB;

        if (dec.errFlags == DECODER_OK) {
            texB.create(dec.resolution.x, dec.resolution.y, dec.format, FilterMode::Nearest, dec.data, false);
        }
        else {
            std::string err;
            std::cout << "Failed to read texture! [" << getBmpErrorString(err, dec.errFlags) << "]\n";
        }
        dec.clear(false);

        JTime time;

        Sprite spriteA("Simba", &texA, float(texA.getSize().y), {0.5f, 0.5f}, JRecti(0, 0, texA.getSize().x, texA.getSize().y));
        Sprite spriteB("Blade", &texB, 32, {0.5f, 0.125f}, JRecti(0, 0, texB.getSize().x, texB.getSize().y));
        Material matA(&texA, &shader);
        Material matB(&texB, &shader);
        Material matBlend(&shaderBlend);

        renderer.setCameraBlendMaterial(&matBlend);

        SimpleRenderer rend[1]{};
        rend[0].setObjectLayer(1);
       
        rend[0].setSprite(&spriteA);
        rend[0].setMaterial(&matA);

        rend[0].setColor({ 255, 255, 255, 255 });
        rend[0].setTRS({ 0, 0 }, 0, { 1, 1 });
 
        Input::init();
        uint64_t connId = Input::addDeviceConnectionCB([](const Input::DeviceIndex index, const bool status) 
            {
                switch (index)
                {
                    default:
                        std::cout << "Unknown Device ";
                        break;
                    case Input::DEV_One:
                        std::cout << "Controller 1 ";
                        break;

                    case Input::DEV_Two:
                        std::cout << "Controller 2 ";
                        break;

                    case Input::DEV_Three:
                        std::cout << "Controller 3 ";
                        break;

                    case Input::DEV_Four:
                        std::cout << "Controller 4 ";
                        break;
                }

                std::cout << "was " << (status ? "Connected!" : "Disconnected!") << std::endl;
            });

        bool debugOpen = true;
        FPS fps;

        DebugCamera cam[3];

        cam[0].setLayerMask(1);
        cam[1].setLayerMask(2);
        cam[2].setLayerMask(2);

        cam[0].setClearColor(JColor32(26, 26, 26, 255));
        cam[0].setCameraTint(JColor32(128, 128, 128, 255));

        cam[1].setClearColor({ 26, 48, 26, 0 });
        cam[1].setCameraTint({0, 0, 0, 96});
        cam[1].setCameraDisabled(true);

        cam[2].setClearColor({26, 48, 26, 0});

        cam[1].setTransformMatrix(JMatrix4f({ 0, 0 }, 0, { 20, 20 }));
        cam[2].setTransformMatrix(JMatrix4f({ 0, 0 }, 0, { 20, 20 }));

        ICamera* camPtrs[3]{
        &cam[0],
        &cam[1],
        &cam[2],
        };

        DebugWindow window(renderer, time, fps, camPtrs, 3, rend, 1);

        static constexpr size_t BLADE_COUNT = 256;
        static constexpr size_t BLADE_COUNT_HALF = BLADE_COUNT >> 1;
        static constexpr size_t BLADE_COUNT_QUAD = BLADE_COUNT >> 2;

        SimpleRenderer blades[BLADE_COUNT];
        for (size_t i = 0; i < BLADE_COUNT; i++) {
            auto& blade = blades[i];

            blade.setObjectLayer(2);
            blade.setMaterial(&matB);
            blade.setSprite(&spriteB);
        }

        static constexpr float START_Y = -8.0f;
        static constexpr float END_Y = 8.0f;
        static constexpr float MID_Y = 0.25f;

        static constexpr float MIN_X = 2.0f;
        static constexpr float MAX_X = -0.5f;
        static constexpr float CENTER_X = 4.0f;

        static constexpr float SPEED = 0.5f;
        static constexpr float FREQUENCY = 6.0f;
        static constexpr float FREQUENCY_ROT = 1.5f;


        static constexpr float ROT_OFF = -180.0f;

        float timeF = 0;
        while (renderer.doRender()) {

            for (size_t i = 0, j = 0; i < BLADE_COUNT_QUAD; i++, j++)
            {
                auto& blade = blades[j];

                const float n = i / (BLADE_COUNT_HALF - 1.0f);
                const float nA = i / (BLADE_COUNT_QUAD - 1.0f);
                const float sin = sinf((timeF * PI * 2.0f * SPEED) + n * PI * FREQUENCY);
                const float sinA = sinf((timeF * PI * 2.0f * SPEED) + n * PI * FREQUENCY * FREQUENCY_ROT);
                const float sinN = (sin + 1.0f) * 0.5f;
                const float sinNA = (sinA + 1.0f) * 0.5f;

                const float y = -MID_Y + Math::lerp(START_Y, END_Y, n);
                const float x = Math::lerp<float>(0, CENTER_X + Math::lerp(MIN_X, MAX_X, sinN), nA * nA * nA);

                const float scale = Math::lerp(0.1f, 1.0f, nA * nA * nA);

                blade.setTRS({ x, y }, 180 - ROT_OFF * sinNA, { scale, scale });
            }

            for (size_t i = 0, j = BLADE_COUNT_QUAD; i < BLADE_COUNT_QUAD; i++, j++)
            {
                auto& blade = blades[j];

                const float n = i / (BLADE_COUNT_HALF - 1.0f);
                const float nA = i / (BLADE_COUNT_QUAD - 1.0f);
                const float sin = sinf((timeF * PI * 2.0f * SPEED) + n * PI * FREQUENCY);
                const float sinA = sinf((timeF * PI * 2.0f * SPEED) + n * PI * FREQUENCY * FREQUENCY_ROT);
                const float sinN = (sin + 1.0f) * 0.5f;
                const float sinNA = (sinA + 1.0f) * 0.5f;

                const float y = MID_Y + Math::lerp(END_Y, START_Y, n);
                const float x = Math::lerp<float>(0, CENTER_X + Math::lerp(MIN_X, MAX_X, sinN), nA * nA * nA);
               
                const float scale = Math::lerp(0.1f, 1.0f, nA * nA * nA);

                blade.setTRS({ x, y }, 0 + ROT_OFF * sinNA, { scale, scale });
                blade.setFlipX(true);
            }

            for (size_t i = 0, j = BLADE_COUNT_QUAD + BLADE_COUNT_QUAD; i < BLADE_COUNT_QUAD; i++, j++)
            {
                auto& blade = blades[j];

                const float n = i / (BLADE_COUNT_HALF - 1.0f);
                const float nA = i / (BLADE_COUNT_QUAD - 1.0f);
                const float sin = sinf((timeF * PI * 2.0f * SPEED) + n * PI * FREQUENCY);
                const float sinA = sinf((timeF * PI * 2.0f * SPEED) + n * PI * FREQUENCY * FREQUENCY_ROT);
                const float sinN = (sin + 1.0f) * 0.5f;
                const float sinNA = (sinA + 1.0f) * 0.5f;

                const float y = -MID_Y + Math::lerp(START_Y, END_Y, n);
                const float x = Math::lerp<float>(0, -CENTER_X + Math::lerp(-MIN_X, -MAX_X, sinN), nA * nA * nA);

                const float scale = Math::lerp(0.1f, 1.0f, nA * nA * nA);

                blade.setTRS({ x, y }, 180 + ROT_OFF * sinNA, { scale, scale });
            }

            for (size_t i = 0, j = BLADE_COUNT_QUAD + BLADE_COUNT_QUAD + BLADE_COUNT_QUAD; i < BLADE_COUNT_QUAD; i++, j++)
            {
                auto& blade = blades[j];

                const float n = i / (BLADE_COUNT_HALF - 1.0f);
                const float nA = i / (BLADE_COUNT_QUAD - 1.0f);
                const float sin = sinf((timeF * PI * 2.0f * SPEED) + n * PI * FREQUENCY);
                const float sinA = sinf((timeF * PI * 2.0f * SPEED) + n * PI * FREQUENCY * FREQUENCY_ROT);
                const float sinN = (sin + 1.0f) * 0.5f;
                const float sinNA = (sinA + 1.0f) * 0.5f;

                const float y = MID_Y + Math::lerp(END_Y, START_Y, n);
                const float x = Math::lerp<float>(0, -CENTER_X + Math::lerp(-MIN_X, -MAX_X, sinN), nA * nA * nA);

                const float scale = Math::lerp(0.1f, 1.0f, nA * nA * nA);

                blade.setTRS({ x, y }, 0 - ROT_OFF * sinNA, { scale, scale });
                blade.setFlipX(true);
            }

            timeF += time.getEngineDeltaTime<float>();
            Input::update(true);
            time.tick();
            fps.update(time);
        }
        //window.removeOnWindowResizeCB(resID);
    }
    renderer.terminate();
    return 0;
}