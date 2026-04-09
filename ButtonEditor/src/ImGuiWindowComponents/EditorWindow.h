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
#include"EditorWindowLayers/RenderLayer.h"
#include"EditorWindowLayers/GuiLayer.h"
#include"ButtonSqrd/Core/ResourceManager.h"


namespace BtnSqd {

	class EditorWindow {
	public:
		EditorWindow(std::shared_ptr<BtnScene>& currentScene);

		void SetScene(std::shared_ptr<BtnScene>& nScene) { currentScene = nScene; }
		void Init();
		void RenderEditor(GameObject& selectedObject);
		void WindowInput();

		void SendEvent(Event* e);

		std::tuple<CameraComponent, TransformComponent> GetEditorCamera() { return std::make_tuple(editorCam, editorCamTransform); }
	private:
		void EditorWindowHotKeys();
		void HandleBtnGui();
		void DragWidget();
		void ScaleWidget();
		bool mousePickRender = false;
		bool firstClick = true;
		bool canDrag = false;
		bool isResizeWidget = false;
		
		std::shared_ptr<BtnWidget> selectedWidget;

		std::shared_ptr<FrameBuffer> editorViewport;
		std::shared_ptr<FrameBuffer> mousePickerBuffer;
		std::shared_ptr<Shader> mousePickerShader;

		CameraComponent editorCam;
		TransformComponent editorCamTransform;

		std::shared_ptr<BtnScene>& currentScene;
		BtnGuiLayer* bGuiLayer;

		ImVec2 windowSize;
		ImVec2 windowPos;

		LayerStack renderLayers;

		//Camera Stuff
		float viewportAspectRatio;
		float lastMouseX=0.0f;
		float lastMouseY=0.0f;
		float yaw = -90.0f;
		float pitch = 0;
	};
}