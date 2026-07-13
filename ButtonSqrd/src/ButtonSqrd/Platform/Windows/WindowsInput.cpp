#include"ButtonSqrd/Core/Input.h"
#include"ButtonSqrd/Core/Application.h"

#include<GLFW/glfw3.h>

namespace BtnSqd {

	 bool Input::IsKeyPressed(KeyCode key) {
		auto* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int>(key));
		return state == GLFW_PRESS;
	 }

	 bool Input::IsKeyUp(KeyCode key) {
		 auto* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		 auto state = glfwGetKey(window, static_cast<int>(key));
		 return state == GLFW_RELEASE;
	 }

	 bool Input::IsMouseButtonPressed(MouseCode button) {
		auto* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int>(button));
		return state == GLFW_PRESS;
	 }
	 
	 bool Input::IsMouseButtonUp(MouseCode button) {
		auto* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int>(button));
		return state == GLFW_RELEASE;
	 }

	 bool Input::IsMouseButtonDoubleClicked(MouseCode button) {
		 auto* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		 auto state = glfwGetMouseButton(window, static_cast<int>(button));
		 int buttonInt = static_cast<int>(button);
		 double cTime = glfwGetTime();
		 double dTime = cTime - mButtonDoubleClick[buttonInt];
		 if (state == GLFW_PRESS) {
			 if (dTime > 0.0 && dTime < 0.25) {
				 mButtonDoubleClick[buttonInt] = 0.0;
				 return true;
			 }
			 else {
				 mButtonDoubleClick[buttonInt] = cTime;
			 }
		 }
		 return false;
	 }

	 glm::vec2 Input::GetMousePosition() {
		auto* window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		double xPos, ypos;
		glfwGetCursorPos(window, &xPos, &ypos);

		return { float(xPos),float(ypos) };
	 }

	 void Input::SetCursorLock(bool locked) {
		 auto* window = static_cast<Window*>(Application::GetApp()->GetWindow());
		 window->SetCursorLock(locked);
	 }
}