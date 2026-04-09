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

	 std::pair<float, float>Input::GetMousePosition() {
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