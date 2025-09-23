#pragma once
#include"ButtonSqrd/Core/core.h"
#include"ButtonSqrd/Core/Window.h"
#include"ButtonSqrd/Core/Input.h"

#include"ButtonSqrd/Event/WindowEvent.h"
#include"ButtonSqrd/Event/KeyEvent.h"
#include"ButtonSqrd/Event/MouseEvent.h"

#include"ButtonSqrd/Render/GraphicsContext.h"

#include<GLFW/glfw3.h>

#include<stdexcept>

namespace BtnSqd {
	class WindowsWindow :public Window
	{
	public:
		//Virtual Functions
		void UpdateImGui() override;
		void DrawImGuiFrame() override;
		void OnUpdate() override;
		Window* GetWindow() override;
		bool ShouldClose() override;
		inline void SetEventCallback(const EventCallbackFn& callback) override { windData.callback = callback; }
		inline void SetCursorLock(bool locked) {
			windData.isLocked = locked;
			locked ? glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED) : glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		//Normal Functions
		WindowsWindow(int m_height, int m_width, const char* m_name, bool isLocked);
		~WindowsWindow();
		inline GLFWwindow* SendWindow() { return window; }
		void* GetNativeWindow() override;
		std::pair<int,int> GetResolution();
		void SetAsCurrent() override;
		bool GetCursorLock() override;

	private:

		static void ResizeFramebufferCallback(GLFWwindow* window, int width, int height);

		struct WindData {
			int width;
			int height;
			const char* name;
			bool isLocked;

			EventCallbackFn callback;
		};

		GLFWwindow* window;
		GraphicsContext* gContext;
		WindData windData;
	};
}