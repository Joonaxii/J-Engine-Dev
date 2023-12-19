#include <JEditor/JEditor.h>

namespace JEngine::Editor {
	bool JEditor::init() {
		if (Application::init()) {
			Input::init<INPUT_SYS_GAME>();
			Input::init<INPUT_SYS_EDITOR>();
			Application::toggleConsoleCloseButton(false);
			//TODO: Load Editor config and set window resolution based on that
			return true;
		}
		return false;
	}

	void JEditor::run() {
		auto& renderer = getRenderer();
		while (true) {
			if (isQuitting()) { break; }

			bool shouldBreak = false;
			shouldBreak |= !renderer.doRender(&_editorWin);

			if (shouldBreak) { break; }
			getTime().tick();

			bool windowHasFocus = this->hasFocus();
			bool editorFocus = _editorWin.isEditorFocused();

			Input::update<INPUT_SYS_GAME>(!editorFocus && windowHasFocus);
			Input::update<INPUT_SYS_EDITOR>(editorFocus && windowHasFocus);
		}
		Application::toggleConsoleCloseButton(true);
	}
}