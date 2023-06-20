#include <JEngine/Core/EntryPoint.h>

namespace JEngine {
	class JEditor : public Application {
	public:
		JEditor(const AppSpecs& specs)
			: Application(specs)
		{
			//TODO: Init
		}

		void run() override {
			auto& renderer = getRenderer();
			while (true) {
				bool shouldBreak = false;



				shouldBreak |= !renderer.doRender();
				if (shouldBreak) { break; }
			}
		}
	private:

	};

	Application* createApplication(AppArgs args) {
		AppSpecs specs;

		char temp[513]{ 0 };
		sprintf_s(temp, "J-Engine V%s [%s]", JENGINE_VERSION.toString().c_str(), JENGINE_VERSION_STR);

		specs.name = std::string(temp);
		specs.args = args;
		return new JEditor(specs);
	}
}