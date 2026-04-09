#pragma once

namespace BtnSqd {
	enum ActiveWindow {
		MainGUI=0,
		GameEditorWindow,
		GameViewportWindow
	};

	enum RuntimeState {
		Playing=0,
		End,
		Pause
	};
}