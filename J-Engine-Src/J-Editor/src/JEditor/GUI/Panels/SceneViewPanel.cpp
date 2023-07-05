#include <JEditor/GUI/Panels/SceneViewPanel.h>
#include <JEngine/Core/Application.h>

namespace JEngine::Editor {
    SceneViewPanel::SceneViewPanel(const char* title, Alignment align) : IDockPanel(title, align) { }

    void SceneViewPanel::draw(Application* app) {
        if (app) {
            void* cam = nullptr;
            if (cam) {
                // const FrameBuffer& fb = cam->getFrameBuffer() ? *cam->getFrameBuffer() : Renderer::getInstance()->getScreenBuffer();
                // auto& specs = fb.getSpecs();
                // auto size = ImGui::GetContentRegionAvail();
                //
                // size.x = std::min<float>(size.x, float(specs.width));
                // size.y = std::min<float>(size.y, float(specs.height));
                //
                // float aspectCam = specs.width / float(specs.height);
                // float aspectPre = size.x / float(size.y);
                //
                // float uv = (1.0f - (aspectPre / aspectCam)) * 0.5f;
                // ImGui::Image((void*)size_t(fb.getColorAttatchment()), size, { uv ,0 }, { 1.0f - uv , 1 });
            }
            else {
                static constexpr char* NO_CAM = "No scene camera!";
                auto avail = ImGui::GetContentRegionAvail();
                auto textS = ImGui::CalcTextSize(NO_CAM);
                ImGui::SetCursorPos({
                     avail.x * 0.5f - textS.x * 0.5f,
                     avail.y * 0.5f - textS.y * 0.5f
                    });
                ImGui::Text(NO_CAM);
            }
        }
    }
}