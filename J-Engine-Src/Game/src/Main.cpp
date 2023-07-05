#include <iostream>
#include <string>
#include <fstream>

#include <JEngine/Assets/Graphics/Shader/Shader.h>
#include <JEngine/Assets/Graphics/Sprite.h>
#include <JEngine/Assets/Graphics/Material/Material.h>

#include <JEngine/Rendering/RenderingConsts.h>
#include <JEngine/Utility/JTime.h>
#include <JEngine/Math/Math.h>

#include <JEngine/Assets/Graphics/Texture/Texture2D.h>
#include <JEngine/IO/Image.h>

#include <JEngine/Rendering/Renderer.h>
#include <JEngine/Rendering/Window.h>

#include <JEngine/Rendering/ImGui/IImGuiDrawable.h>
#include <SimpleRenderer.h>
#include <imgui.h>
#include <JEngine/Debug/FPS.h>

using namespace JEngine;

static const std::string SHADER_PATH = "../../../Game/res/shaders/";
static const std::string TEXTURE_PATH = "../../../Game/res/textures/";

struct DebugWindow : public IImGuiDrawable {
    Renderer& renderer;
    JTime& timeRef;
    FPS& fps;

    DebugWindow(Renderer& renderer, JTime& timeRef, FPS& fpsRef) : IImGuiDrawable(), timeRef(timeRef), fps(fpsRef), renderer(renderer) {}
    ~DebugWindow() { IImGuiDrawable::~IImGuiDrawable(); }

    void onImGui() override { 
        const auto& rendInf = renderer.getRenderInfo();
        if (ImGui::Begin("Debug Info")) {
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
            }
        }
        ImGui::End();
    }

};

int main() {
    Renderer renderer;
    {
        JTime time;

        ImageData bmpRes{};
        Texture texA{};
        if (Bmp::decode(TEXTURE_PATH + "Simba.bmp", bmpRes)) {
            texA.create(bmpRes, FilterMode::Nearest);
            bmpRes.clear(false);
        }
    
        Texture texB{};
        if (Bmp::decode(TEXTURE_PATH + "Blade.bmp", bmpRes)) {
            texB.create(bmpRes, FilterMode::Nearest);
            bmpRes.clear(false);
        }

        Texture texC{};
        if (Bmp::decode(TEXTURE_PATH + "Sus.bmp", bmpRes)) {
            texC.create(bmpRes, FilterMode::Nearest);
            bmpRes.clear(false);
        }

        Shader shaderA(SHADER_PATH + "Indexed.shader");
        Shader shaderB(SHADER_PATH + "Textured.shader");

        float rotation = 0;
        JVector2f position(0, 0);
        JMatrix4f mat;

        Sprite spriteA("Test A", ObjectRef<Texture>(&texA), 128, JVector2f(0.5f, 0.5f), JRecti(0, 0, texA.getWidth(), texA.getHeight()));
        Sprite spriteB("Test B", ObjectRef<Texture>(&texB), 32, JVector2f(0.5f, 0.0f), JRecti(0, 0, texB.getWidth(), texB.getHeight()));
        Sprite spriteC("Test C", ObjectRef<Texture>(&texC), 128, JVector2f(0.5f, 0.5f), JRecti(0, 0, texC.getWidth(), texC.getHeight()));

        Material materials[3]{
            Material(&texB, &shaderB),
            Material(&texA, &shaderB),
            Material(&texC, &shaderA),
        };

        static constexpr int BLADES_QUADRANTS = 8;
        static constexpr int BLADES_QUAD = 256;
        SimpleRenderer renderers[BLADES_QUAD * BLADES_QUADRANTS]{SimpleRenderer()};

        for (size_t i = 0; i < BLADES_QUAD * BLADES_QUADRANTS; i++) {
            renderers[i].setMaterial(&materials[0]);
            renderers[i].setSprite(&spriteB);
        }

        float zoom = 1.0f;
        time.reset();

        static constexpr float BLADE_AMP = 0.5f;
        static constexpr float BLADE_FREQ = 0.25f;
        static constexpr float BLADE_SPEED = 0.25f;
        static constexpr float BLADE_WIDTH = 9.5f;
        static constexpr float BLADE_ROT_OFFSET = -45;
        static constexpr float BLADE_ROT_MAX = 360.0f;

        static constexpr float ROT_OFFSET = 360.0f / BLADES_QUADRANTS;

        static constexpr float len = BLADES_QUAD < 3 ? 1.0f : (BLADES_QUAD - 1.0f);
        FPS fps;

        DebugWindow debugWin(renderer, time, fps);
        while (renderer.doRender(nullptr)) {

            float timeT = float(time.getTime(0) * PI * 2.0 * BLADE_SPEED);
            JVector2f pos;

            const float degPerI = (360.0f / (BLADES_QUAD * BLADES_QUADRANTS));
            const float radPerI = degPerI * DEG_2_RAD;

            float rot = 0;
            float rotDeg = 0;
            int32_t cur = 0;
            JVector2f off;

            float rotO = 0;
            for (size_t k = 0; k < BLADES_QUADRANTS; k++)
            {
                for (size_t i = 0; i < BLADES_QUAD; i++, cur++) {
                    auto& rendA = renderers[cur];
                    const float n = i / len;
                    const float NN = n * n * n;
                    const float sin = (sinf(timeT + (NN * BLADE_FREQ * PI)));

                    off = { cosf(rot) * BLADE_WIDTH, sinf(rot) * BLADE_WIDTH };
                    //rendA.setTRS(off + Math::rotateRad(JVector2f(0, (sin + 2.0f) * 0.5f * BLADE_AMP), rot), rot + sin * BLADE_ROT_MAX + rotO + BLADE_ROT_OFFSET, JVector2f(1, 1));

                    rot += radPerI;
                }
                rotO += ROT_OFFSET;
            }

            float delta = float(time.getDeltaTime(0));
            rotation += 90 * delta;
            rotation = Math::repeat(rotation, 360.0f);


            time.tick();
            fps.update(time);
        }
    }

    renderer.terminate();
    return 0;
}