#pragma once

#include<ButtonSqrd.h>
#include<imgui.h>
#include<ImGuizmo.h>

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include"../EditorEvents.h"
#include"GameWindowLayers/GameLayer.h"
#include"ButtonSqrd/Gui/BtnGuiLayer.h"

#include"ButtonSqrd/Core/ResourceManager.h"

namespace BtnSqd {	
	class GameWindow {
	public:
		GameWindow(std::shared_ptr<BtnScene>&scene);

		void SendEvent(Event* e);
		void RenderWindow();
	private:
		std::shared_ptr<FrameBuffer> gameViewport;

		std::shared_ptr<BtnScene>& currentScene;

		LayerStack renderLayers;
	};
}
