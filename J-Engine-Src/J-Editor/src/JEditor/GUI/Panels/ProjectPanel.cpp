#include <JEditor/GUI/Panels/ProjectPanel.h>

namespace JEngine::Editor {
    ProjectPanel::ProjectPanel(const char* title, Alignment align) : IDockPanel(title, align) { }
    void ProjectPanel::init(Application* app) {
    }

    static void appendBuffer(std::string& str, int32_t depth) {
        if (depth > 0) {
            for (size_t i = 1; i < depth; i++) {
                str.append(1, '|');
                str.append(2, ' ');
            }
        }
    }

    static void printEntry(const FileEntry* entry, std::vector<std::string>& indents, int32_t depth) {
        JENGINE_CORE_TRACE("{0}{1}", indents[depth].c_str(), entry->name.c_str());

        depth++;
        if (indents.size() <= depth) {
            std::string& indent = indents.emplace_back("");
            appendBuffer(indent, depth);
            indent.append(1, '|');
            indent.append(2, '-');
            indent.append(1, '|');
        }
  
        for (const auto& ent : entry->children) {
            printEntry(ent, indents, depth);
        }

        if (entry->children.size() > 0 && entry->parent) {
            std::string indent = "";
            appendBuffer(indent, depth);
            indent.append(4, ' ');
            JENGINE_CORE_TRACE("{0}", indent.c_str());
        }
    }

    void ProjectPanel::draw(Application* app) {
        if (app) {
            const auto vfsEdi = app->getAssetDB().getVFS(ResourceRoot::RT_EDITOR);
            static constexpr float BUFFER = 5;
            ImVec2 size = ImGui::GetContentRegionAvail();
            size.x *= 0.5f;
            size.y = ImGui::GetTextLineHeight() * 5;

            if (vfsEdi) {
                ImGui::BeginChild("Editor##ProjectRes", size);
                ImGui::PushID("Editor");
                ImGui::Text("[Editor Resources]");
                ImGui::Text(" - Asset Root    : '%s'", vfsEdi->getRootPath().c_str());
                ImGui::Text(" - Total Entries : '%zi'", vfsEdi->getEntryCount());
                ImGui::Text(" - Total Files   : '%zi'", vfsEdi->getFileCount());
                ImGui::PopID();
                ImGui::EndChild();
            }

            const auto vfsGam = app->getAssetDB().getVFS(ResourceRoot::RT_GAME);
            if (vfsGam) {
                ImGui::SameLine();
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
                ImGui::SameLine();
                ImGui::BeginChild("Game##ProjectRes", size); 
                ImGui::PushID("Game");
                ImGui::Text("[Game Resources]");
                ImGui::Text(" - Asset Root      : '%s'", vfsGam->getRootPath().c_str());
                ImGui::Text(" - Total Entries   : '%zi'", vfsGam->getEntryCount());
                ImGui::Text(" - Total Files     : '%zi'", vfsGam->getFileCount());
                ImGui::PopID();
                ImGui::EndChild();
            }

            if (ImGui::Button("Build VFS##Project")) {
                app->getAssetDB().buildVFS(0x3);
            }

            ImGui::BeginDisabled(!vfsEdi);
            if (ImGui::Button("Print Editor VFS##Project") && vfsEdi) {
                const FileEntry* entry = vfsEdi->getRoot();
                std::vector<std::string> indents{"|"};
                JENGINE_CORE_TRACE("[Editor Directory Tree]");
                printEntry(entry, indents, 0);
            }
            ImGui::EndDisabled();

            ImGui::BeginDisabled(!vfsGam);
            if (ImGui::Button("Print Game VFS##Project") && vfsGam) {
                const FileEntry* entry = vfsGam->getRoot();
                std::vector<std::string> indents{ "|" };
                JENGINE_CORE_TRACE("[Game Directory Tree]");
                printEntry(entry, indents, 0);
            }
            ImGui::EndDisabled();
        }
    }

    void ProjectPanel::close(Application* app)
    {
    }
}