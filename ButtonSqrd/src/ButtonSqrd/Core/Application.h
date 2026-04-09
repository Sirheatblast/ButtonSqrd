#pragma once
#include"core.h"
#include"Logger.h"
#include"Window.h"
#include"Input.h"
#include"Layer.h"
#include"LayerStack.h"
#include"BtnTime.h"
#include"GameObject.h"
#include"SuperGameObject.h"
#include"CoreComponents.h"
#include"Scene.h"
#include"RayCaster.h"

#include"ButtonSqrd/Event/Event.h"
#include"ButtonSqrd/Event/WindowEvent.h"
#include"ButtonSqrd/Event/KeyEvent.h"
#include"ButtonSqrd/Event/MouseEvent.h"

#include"ButtonSqrd/Render/Renderer.h"
#include"ButtonSqrd/Render/RendererCommand.h"
#include"ButtonSqrd/Render/VertexArray.h"
#include"ButtonSqrd/Render/Buffers.h"
#include"ButtonSqrd/Render/FrameBuffer.h"
#include"ButtonSqrd/Render/Shader.h"

#include<memory>

namespace BtnSqd {
	struct ViewPort {
		glm::vec2 size;
		glm::vec2 offset = glm::vec2();
	};

	class Application{
	public:
		Application();
		~Application();

		void RunApp();	
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PushEvent(Event* evnt) { OnEvent(evnt); }

		Window* GetWindow() { return window; }
		void SetWindow(Window*& nWind) {
			if (window) {
				delete window;
			}
			window = nWind;
		}

		ViewPort GetCurrentViewPort()const { return currentViewPort; }
		void SetViewPort(ViewPort viewPort) {
			currentViewPort = viewPort;
		}

		static Application* GetApp() { return app; }
		static Application*& GetAppRef() { return app; }

	private:
		void OnEvent(Event* evnt);

		LayerStack layerStack;
		Window* window;
		ViewPort currentViewPort;
		static Application* app;
		BtnScene activeScene;
	};
	//To be defined in client
	Application* CreateApp();	
}
