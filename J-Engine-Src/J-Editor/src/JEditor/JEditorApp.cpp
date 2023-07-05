#include <JEngine/Core/EntryPoint.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/Components/CTransform.h>
#include <JEngine/Components/ComponentFactory.h>
#include <JEditor/GUI/MainWindow.h>

namespace JEngine {
	namespace Editor {

		class JEditor : public Application {
		public:
			JEditor(const AppSpecs& specs, const ResourceRoot* roots, size_t rootCount)
				: Application(specs, roots, rootCount), _editorWin(this)
			{
				//TODO: Init
			}

			void run() override {
				auto& renderer = getRenderer();
				while (true) {
					if (isQuitting()) { break; }

					bool shouldBreak = false;
					shouldBreak |= !renderer.doRender(&_editorWin);

					if (shouldBreak) { break; }
					getTime().tick();
					Input::update(true);
				}
			}
		private:
			MainWindow _editorWin;
		};
	};

	Application* createApplication(AppArgs args) {
		AppSpecs specs;

		char temp[513]{ 0 };
		sprintf_s(temp, "J-Engine V%s [%s]", JENGINE_VERSION.toString().c_str(), JENGINE_VERSION_STR);

		specs.name = std::string(temp);
		specs.workingDir = IO::getExeDir();
		specs.args = args;

		ResourceRoot roots[2]{
			ResourceRoot(GAME_RES_PATH, ResourceRoot::RT_GAME),
			ResourceRoot(JEDITOR_RES_PATH, ResourceRoot::RT_EDITOR),
		};

		return new Editor::JEditor(specs, roots, 2);
	}
}