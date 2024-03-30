#include <JEditor/GUI/Panels/ProjectPanel.h>
#include <JEngine/Core/Application.h>

namespace JEngine::Editor {
    static constexpr const char* getAssetSourceName(uint8_t source) {
        switch (source)
        {
            case AssetDB::SRC_BUILT_IN: return "Built-In";
            case AssetDB::SRC_EDITOR: return "Editor";
            case AssetDB::SRC_GAME: return "Game";
            case AssetDB::SRC_RUNTIME: return "Runtime";
            case AssetDB::SRC_OVERRIDE: return "Override";
            default: return "Unknown";
        }
    }

    static void drawEntry(FileEntry* entry) {
        if (!entry) { return; }

        ImGui::PushID(entry);

        //TODO: Fetch actual icon when Texture2D and Sprites are implemented
        ImGui::Image(0, { ImGui::GetFrameHeight(), 0});
        ImGui::SameLine();
        if (entry->isFolder()) {
            if (ImGui::CollapsingHeader(entry->name.c_str())) {
                ImGui::Indent();

                ImGui::Unindent();
            }
        }
        else {
            ImGui::Text(entry->name.c_str());
        }
        ImGui::PopID();
    }

    static void drawVFSItem(AssetDB& db, uint8_t source, float buffer = 5) {
        if (source >= AssetDB::SRC_COUNT) { return; }
        static constexpr float BUTTON_SIZE = 80;
        static constexpr float MIN_SIZE = 80;

        float width = (ImGui::GetContentRegionAvail().x - BUTTON_SIZE) - buffer;

        if (width >= MIN_SIZE) {
            ImGui::SetNextItemWidth(BUTTON_SIZE);
            ImGui::BeginDisabled(source >= AssetDB::SRC_RUNTIME);
            if (ImGui::Button("Refresh")) {
                //TODO: Refresh current asset source, if possible
            }
            ImGui::EndDisabled();
            ImGui::SameLine();
        }

        bool hdr = ImGui::CollapsingHeader(getAssetSourceName(source));
        if (hdr) {
            ImGui::Indent();
            //TODO: Show list of VFS...


           
            ImGui::Unindent();
        }
    }

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

    static void printEntry(const VFS& vfs, const FileEntry* entry, std::vector<std::string>& indents, int32_t depth) {
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
            printEntry(vfs, vfs.getEntryByRef(ent), indents, depth);
        }

        if (entry->children.size() > 0 && entry->parent.isValid()) {
            std::string indent = "";
            appendBuffer(indent, depth);
            indent.append(4, ' ');
            JENGINE_CORE_TRACE("{0}", indent.c_str());
        }
    }

    void ProjectPanel::draw(Application* app) {
        if (app) {
            auto& assetDB = app->getAssetDB();
            auto vfsEdi = app->getAssetDB().getVFS(AssetDB::SRC_EDITOR);
            static constexpr float BUFFER = 5;
            ImVec2 size = ImGui::GetContentRegionAvail();
            size.x *= 0.333f;
            size.y = ImGui::GetTextLineHeight() * 5;

            if (ImGui::BeginChild("##FileSideBar", { 250, 0 }, ImGuiChildFlags_ResizeX)) {
                if (ImGui::BeginChild("##FileSideBar_V2", { ImGui::GetContentRegionAvail().x - 5, 0}, ImGuiChildFlags_None)) {
                    for (uint8_t i = 0; i < AssetDB::SRC_COUNT; i++) {
                        ImGui::PushID(i);
                        drawVFSItem(assetDB, i, 10);
                        ImGui::PopID();
                    }
                    ImGui::EndChild();
                }
            }
            ImGui::EndChild();
            ImGui::SameLine(0.0f, 0.0f);
            ImGui::SeparatorEx(ImGuiSeparatorFlags_SpanAllColumns | ImGuiSeparatorFlags_Vertical);
            ImGui::SameLine();
            if (ImGui::BeginChild("##FileSystem", { 0, 0 }, ImGuiChildFlags_None)) {

            }
            ImGui::EndChild();
        }
    }

    void ProjectPanel::close(Application* app)
    {
    }
}