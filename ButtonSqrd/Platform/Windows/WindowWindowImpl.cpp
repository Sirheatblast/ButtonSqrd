#include "WindowsWindowImpl.h"

#include"ButtonSqrd/Platform/OpenGL/OpenGLContext.h"
#include"ButtonSqrd/Render/RendererCommand.h"

namespace BtnSqd {
	Window* Window::CreateWindowFunction(const char* m_name, bool isLocked,int m_width, int m_height) {
		return new WindowsWindow(m_width, m_height, m_name,isLocked);
	}

	void WindowsWindow::UpdateImGui() {
		gContext->InitImgui();
	}

	void WindowsWindow::DrawImGuiFrame(){
		gContext->DrawImGuiToApi();
	}

	void WindowsWindow::OnUpdate() {
		if (windData.isLocked)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		gContext->SwapBuffers();
		glfwPollEvents();
	}

	WindowsWindow::WindowsWindow(int width, int height, const char* name, bool isLocked){
		glfwInit();
		
		if (width ==-1||height ==-1) {
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			width = mode->width;
			height = mode->height;
		}

		windData.height = height;
		windData.width = width;
		windData.isLocked = isLocked;

		window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (window == nullptr) {
			throw std::runtime_error("The Window wasn't created properly");
			glfwTerminate();
		}
		gContext = new OpenGLContext(window);
		gContext->Init();

		glfwWindowHint(GLFW_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &windData);

		//Setup callbacks
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;
			RenderCommand::ResizeScreen(width, height);
		});

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			data.callback(new OnWindowResizeEvent(data.width, data.height, 1));
		});
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			data.callback(new OnWindowCloseEvent(1));
		});
		glfwSetWindowPosCallback(window, [](GLFWwindow* window, int posX, int posY) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			data.callback(new OnWindowMoveEvent(posX, posY));
		});
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			switch (action) {
			case GLFW_PRESS:
				data.callback(new OnKeyDownEvent(static_cast<KeyCode>(key)));
				break;
			case GLFW_RELEASE:
				data.callback(new OnKeyReleaseEvent(static_cast<KeyCode>(key)));
				break;
			case GLFW_REPEAT:
				data.callback(new OnKeyHoldEvent(static_cast<KeyCode>(key)));
				break;
			}
		});
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			switch (action) {
			case GLFW_PRESS:
				data.callback(new OnMouseClickEvent(button));
				break;
			case GLFW_RELEASE:
				data.callback(new OnMouseClickReleaseEvent(button));
				break;
			}
		});
		glfwSetCursorPosCallback(window, [](GLFWwindow* window,double xpos,double ypos ) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			data.callback(new OnMouseMoveEvent(float(xpos), float(ypos)));
		});
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			data.callback(new OnMouseScrollEvent(float(xoffset), float(yoffset)));
		});
		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keyCode) {
			WindData& data = *(WindData*)glfwGetWindowUserPointer(window);
			data.callback(new OnKeyTypedEvent(keyCode));
		});
	}

	Window* WindowsWindow::GetWindow() {
		return this;
	}

	bool WindowsWindow::ShouldClose() {
		return glfwWindowShouldClose(window);
	}

	WindowsWindow::~WindowsWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void* WindowsWindow::GetNativeWindow()	{
		return static_cast<void*>(SendWindow());
	}
	 std::pair<int,int>WindowsWindow::GetResolution(){
		return std::make_pair(windData.width,windData.height);
	}
	 void WindowsWindow::ResizeFramebufferCallback(GLFWwindow* window, int width, int height) {
		
	 }
	 bool WindowsWindow::GetCursorLock() {
		 return windData.isLocked;
	 }
	 void WindowsWindow::SetAsCurrent() {
		 glfwMakeContextCurrent(window);
	 }
}