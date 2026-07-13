#pragma once
#include"core.h"

#include"KeyCodes.h"
#include"MouseButtonCodes.h"

#include<unordered_map>
#include<glm/glm.hpp>

namespace BtnSqd {
	enum KeyState {
		Nutral = 0, Pushed, Released, Held
	};

	class Input {
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsKeyUp(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static bool IsMouseButtonUp(MouseCode button);
		static bool IsMouseButtonDoubleClicked(MouseCode button);

		static glm::vec2 GetMousePosition();

		static void SetCursorLock(bool locked);

	private:
		static inline double mButtonDoubleClick[8] = {0.0};
	};
}