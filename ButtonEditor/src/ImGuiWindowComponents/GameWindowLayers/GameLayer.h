#pragma once
#include<ButtonSqrd.h>
#include<imgui.h>
#include<ImGuizmo.h>

#include"../../EditorEvents.h"
#include"../../EditorFlags.h"
#include"ButtonSqrd/Event/CameraEvent.h"
#include"ButtonSqrd/Core/ResourceManager.h"
#include"ButtonSqrd/Gui/BtnGuiLayer.h"

namespace BtnSqd {
	class GameLayer : public Layer {
	public:
		GameLayer(std::shared_ptr<BtnScene>& scene, std::shared_ptr<FrameBuffer> editorViewport);
		~GameLayer()  override;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event* e) override;
	private:
		bool OnSelectCamera(OnSetActiveCameraEvent* e);

		GameObject activeCamera;

		std::shared_ptr<BtnScene>& currentScene;
		std::shared_ptr<FrameBuffer> gameViewport;
		std::shared_ptr<BtnGuiLayer> guiLayer;

		float lastWindowX = 0.0f;
		float lastWindowY = 0.0f;
		float imageOffset = 50.0f;
		RuntimeState currentState;
	};
}