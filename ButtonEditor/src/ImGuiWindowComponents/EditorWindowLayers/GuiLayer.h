#pragma once
#include<ButtonSqrd.h>
#include<imgui.h>
#include<ImGuizmo.h>

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include"../../EditorEvents.h"
#include"ButtonSqrd/Core/ResourceManager.h"

namespace BtnSqd {
	class GuiLayer: public Layer {
		enum class GizmoMode {
			Transform,
			Rotation,
			Scale
		};
	public:
		GuiLayer(std::tuple<CameraComponent*, TransformComponent*> camera);
		~GuiLayer()  override;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event* e) override;
	private:
		void DrawGui();

		bool OnSelectGameObject(OnSelectGameObjectEvent* e);
		bool OnDesroyGameObject(OnDesroyGameObjectEvent* e);

		bool showColliders = false;

		GizmoMode gizmoMode = GizmoMode::Transform;
		GameObject selectedObject;

		CameraComponent* editorCam;
		TransformComponent* editorCamTransform;
	};
}