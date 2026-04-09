#include "Application.h"

#include"ButtonSqrd/Render/Model.h"

namespace BtnSqd {
	Application* Application::app = nullptr;
	Application::Application():activeScene(BtnScene("New Scene")) {
		app = this;
		window = Window::CreateWindowFunction("Button Sqrd Engine",false);
		//Init stage
		auto [width, height] = window->GetResolution();
		currentViewPort = ViewPort(glm::vec2((float)width,(float)height));
		BtnSqd::Logger::Init();
		window->SetEventCallback(std::bind(&Application::OnEvent,this,std::placeholders::_1));
	}

	Application::~Application() {
		delete window;
	}

	void Application::RunApp() {

		while (window->ShouldClose()!=true) {
			BtnTime::CalculateDeltaTimeStart();
			 window->OnUpdate();
			 RenderCommand::Clear();
			 RenderCommand::SetClearColor(glm::vec4(0.0f, 0.25f, 0.5f, 1.0f));

			 
			 for (Layer* layer : layerStack) {
				 layer->OnUpdate();
			 }

			 BtnTime::CalculateDeltaTimeEnd();
		}
	}
	 void Application::PushLayer(Layer* layer){
		 layerStack.PushLayer(layer);
	 }
	 void Application::PushOverlay(Layer* layer){
		 layerStack.PushOverlay(layer);
	 }
	 void Application::OnEvent(Event* evnt) {	 
		 for (Layer* layer : layerStack) {
			 layer->OnEvent(evnt);
		 }
	 }
}