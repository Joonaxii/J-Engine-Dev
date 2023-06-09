#include <JEngine/Core/EntryPoint.h>

namespace JEngine {
	class JEditor : public Application {
	public:
		JEditor(const AppSpecs& specs)
			: Application(specs)
		{
			//TODO: Init
		}
	};

	Application* createApplication(AppArgs args) {
		AppSpecs specs;
		specs.name = "J-Editor";
		specs.args = args;
		return new JEditor(specs);
	}
}