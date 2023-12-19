#include <JEngine/Core/EntryPoint.h>
#include <JEditor/JEditor.h>

namespace JEngine {

	Application* createApplication(AppArgs args) {
		AppSpecs specs;

		char temp[513]{ 0 };
		sprintf_s(temp, "J-Engine V%s [%s]", JENGINE_VERSION.toString().c_str(), JENGINE_VERSION_STR);

		specs.name = std::string(temp);
		specs.workingDir = IO::getExeDir();
		specs.args = args;

		ConstSpan<char> roots[JEngine::AssetDB::AssetSourceType::SRC_COUNT]{
			JENGINE_RES_PATH,
			JEDITOR_RES_PATH,
			GAME_RES_PATH,
			"",
			"/res"
		};

		ConstSpan<char> rootsDB[JEngine::AssetDB::AssetSourceType::SRC_COUNT]{
			JENGINE_DB_PATH,
			JEDITOR_DB_PATH,
			GAME_DB_PATH,
			"",
			""
		};

		Editor::JEditor* editor = new Editor::JEditor(specs);
		editor->getAssetDB().initialize(roots, rootsDB);
		return editor;
	}
}