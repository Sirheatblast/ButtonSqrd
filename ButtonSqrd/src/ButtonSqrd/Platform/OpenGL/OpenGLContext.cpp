#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "OpenGLContext.h"

BtnSqd::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):window(windowHandle){

}

void BtnSqd::OpenGLContext::SwapBuffers(){
	glfwSwapBuffers(window);
}

void BtnSqd::OpenGLContext::Init(){
	glfwMakeContextCurrent(window);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		  // Enable Docking

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420");
}

void BtnSqd::OpenGLContext::InitImgui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
}

void BtnSqd::OpenGLContext::DrawImGuiToApi() {
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void BtnSqd::OpenGLContext::ResizeCallback(unsigned int width, unsigned int height) {
	glViewport(0, 0, width, height);
}
