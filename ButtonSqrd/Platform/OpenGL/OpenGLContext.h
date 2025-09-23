#pragma once
#include"ButtonSqrd/Render/GraphicsContext.h"

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#include<GLFW/glfw3.h>
#include<glad/glad.h>
#include"ButtonSqrd/Core/KeyCodes.h"

struct GLFWwindow;

namespace BtnSqd {
	class OpenGLContext:public GraphicsContext{

	public:
		OpenGLContext(GLFWwindow* windowHandle);
		void SwapBuffers() override;
		void Init() override;
		void InitImgui() override;
		void DrawImGuiToApi() override;

	private:
		GLFWwindow* window;
		void ResizeCallback(unsigned int width, unsigned int height) override;
	};
}