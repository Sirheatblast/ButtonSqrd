#pragma once
#include"core.h"

#include"KeyCodes.h"
#include"MouseButtonCodes.h"

#include<unordered_map>
namespace BtnSqd {
	enum KeyState {
		Nutral=0,Pushed, Released, Held
	};

	class Input {
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsKeyUp(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static bool IsMouseButtonUp(MouseCode button);

		static std::pair<float, float>GetMousePosition();

		static void SetCursorLock(bool locked);

	};
}