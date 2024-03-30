#pragma once
#include <JEngine/Core/Application.h>
#include <JEditor/GUI/MainWindow.h>

namespace JEngine::Editor {
	class JEditor : public Application {
	public:
		enum InputSource : size_t {
			INPUT_SYS_GAME = 0x0,
			INPUT_SYS_EDITOR = 0x1,
		};

		class EditorCamera : public ICamera {
			EditorCamera() : ICamera(AUTO_RENDER_CAMERA | CAMERA_NO_REGISTRATION) {}

			void setTransformMatrix(const JMatrix4f&) override;
			const JMatrix4f& getTransformMatrix() const override;
		};

		enum : uint16_t {
			EDITOR_FL_IN_PLAY_MODE = 0x1,
		};

		JEditor(const AppSpecs& specs)
			: Application(specs), _editorWin(this) { }

		bool init() override;
		void run() override;

	private:
		UI16Flags _flags;
		MainWindow _editorWin;
	};
}
