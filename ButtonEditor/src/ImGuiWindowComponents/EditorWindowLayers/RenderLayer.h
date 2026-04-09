#pragma once
#include<ButtonSqrd.h>
#include<imgui.h>
#include<ImGuizmo.h>

#include"../../EditorEvents.h"
#include"ButtonSqrd/Core/ResourceManager.h"
#include"ButtonSqrd/Gui/BtnGuiLayer.h"

namespace BtnSqd {
	class RenderLayer : public Layer {
	public:
		RenderLayer(std::shared_ptr<BtnScene>& scene, std::shared_ptr<FrameBuffer> editorViewport,std::tuple<CameraComponent*,TransformComponent*> camera);
		~RenderLayer()  override;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void HandleCreateSuperGameObj(BtnSqd::AssetPayloadType& assetInfo);
		void HandleCreateModel(BtnSqd::AssetPayloadType& assetInfo);
		void OnEvent(Event* e) override;
	private:
		void SelectObject();
		bool OnSelectWindow(OnSelectWindowEvent* e);

		CameraComponent* editorCam;
		TransformComponent* editorCamTransform;

		std::shared_ptr<BtnScene>& currentScene;
		std::shared_ptr<FrameBuffer> editorViewport;
		std::shared_ptr<FrameBuffer> mousePickerBuffer;
		std::shared_ptr<Shader> mousePickerShader;
		std::shared_ptr<BtnGuiLayer> guiLayer;

		bool windowSelected = false;
		bool canMousePick = true;

		float lastWindowX=0.0f;
		float lastWindowY=0.0f;
	};
}