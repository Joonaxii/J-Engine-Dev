#include <JEngine/Core/EntryPoint.h>
#include <JEngine/Core/Application.h>

namespace JEngine {

	class JPlayer : public Application {
	public:

		JPlayer(const AppSpecs& specs)
			: Application(specs) { }

		bool init() override {
			if (Application::init()) {
#ifndef JE_DEBUG
				hideConsole();
#endif
				return true;
			}
			return false;
		}
		void run() override {
			auto& renderer = getRenderer();
			while (true) {
				if (isQuitting()) { break; }

				bool shouldBreak = false;
				shouldBreak |= !renderer.doRender(nullptr);

				if (shouldBreak) { break; }
				getTime().tick();

				bool windowHasFocus = this->hasFocus();
				Input::update<0>(windowHasFocus);
			}
		}
	};

	Application* createApplication(AppArgs args) {
		AppSpecs specs;

		char temp[513]{ 0 };
		sprintf_s(temp, "J-Player V%s [%s]", JE_VERSION.toString().c_str(), JE_VERSION_STR);

		specs.name = std::string(temp);
		specs.workingDir = IO::getExeDir();
		specs.args = args;

		//ConstSpan<char> roots[JEngine::AssetDB::AssetSourceType::SRC_COUNT]{
		//	JE_RES_PATH,
		//	JEDITOR_RES_PATH,
		//	GAME_RES_PATH,
		//	""
		//};
		//
		//ConstSpan<char> rootsDB[JEngine::AssetDB::AssetSourceType::SRC_COUNT]{
		//	JE_DB_PATH,
		//	JEDITOR_DB_PATH,
		//	GAME_DB_PATH,
		//	""
		//};

		JPlayer* player = new JPlayer(specs);
		//editor->getAssetDB().initialize(roots, rootsDB);
		return player;
	}
}