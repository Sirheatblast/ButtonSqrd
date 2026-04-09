#include "GuiLayer.h"

BtnSqd::GuiLayer::GuiLayer(std::tuple<CameraComponent*, TransformComponent*> camera){
	auto [camComp, transComp] = camera;
	editorCam = camComp;
	editorCamTransform = transComp;
}

BtnSqd::GuiLayer::~GuiLayer() {

}

void BtnSqd::GuiLayer::OnAttach() {

}

void BtnSqd::GuiLayer::OnDetach() {

}

void BtnSqd::GuiLayer::OnUpdate() {
	ImGui::Begin("Editor Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	DrawGui();
	if (selectedObject.IsActive()) {
		if (Input::IsMouseButtonUp(MouseCode::Right)) {
			if (Input::IsKeyPressed(KeyCode::G)) {
				gizmoMode = GizmoMode::Transform;
			}
			if (Input::IsKeyPressed(KeyCode::R)) {
				gizmoMode = GizmoMode::Rotation;
			}
			if (Input::IsKeyPressed(KeyCode::S)) {
				gizmoMode = GizmoMode::Scale;
			}
		}

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

		glm::mat4* matrix = nullptr;
		ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;
		
		switch (gizmoMode) {
		case GizmoMode::Transform:
			matrix = &selectedObject.GetComponent<TransformComponent>().transform.GetTransformMat();
			operation = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case GizmoMode::Rotation:
			glm::mat4 rotationMat = selectedObject.GetComponent<TransformComponent>().transform.GetMatrix();
			matrix = &rotationMat;
			operation = ImGuizmo::OPERATION::ROTATE;
			break;
		case GizmoMode::Scale:
			glm::mat4 scale = selectedObject.GetComponent<TransformComponent>().transform.GetMatrix();
			matrix = &scale;
			operation = ImGuizmo::OPERATION::SCALE;
			break;
		}

		if (matrix) {
			ImGuizmo::Manipulate(glm::value_ptr(editorCam->viewMatrix), glm::value_ptr(editorCam->projectionMatrix), operation, ImGuizmo::LOCAL, glm::value_ptr(*matrix));

			if (ImGuizmo::IsUsing()) {
				if (gizmoMode == GizmoMode::Rotation) {
					glm::vec3 skew, scale, position;
					glm::vec4 perspective;
					glm::quat rotation;
					glm::decompose(*matrix, scale, rotation, position, skew, perspective);

					selectedObject.GetComponent<TransformComponent>().transform.RotateTo(rotation);
				}
				else if (gizmoMode == GizmoMode::Scale) {
					float scaleX = glm::sqrt(matrix[0][0].x * matrix[0][0].x + matrix[0][0].y * matrix[0][0].y + matrix[0][0].z * matrix[0][0].z);
					float scaleY = glm::sqrt(matrix[0][1].x * matrix[0][1].x + matrix[0][1].y * matrix[0][1].y + matrix[0][1].z * matrix[0][1].z);
					float scaleZ = glm::sqrt(matrix[0][2].x * matrix[0][2].x + matrix[0][2].y * matrix[0][2].y + matrix[0][2].z * matrix[0][2].z);
					selectedObject.GetComponent<TransformComponent>().transform.Scale(glm::vec3(scaleX, scaleY, scaleZ));
				}
			}
		}
	}
	ImGui::End();
}

void BtnSqd::GuiLayer::OnEvent(Event* e) {
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<BtnSqd::OnSelectGameObjectEvent>(std::bind(&GuiLayer::OnSelectGameObject, this, std::placeholders::_1));
	dispatcher.Dispatch<BtnSqd::OnDesroyGameObjectEvent>(std::bind(&GuiLayer::OnDesroyGameObject, this, std::placeholders::_1));
}

void BtnSqd::GuiLayer::DrawGui() {
	ImVec2 startGuiPos = ImVec2(0.0f,20.0f);
	ImGui::SetCursorPos(startGuiPos);
	if (ImGui::Button("Transform")) {
		gizmoMode = GizmoMode::Transform;
	}
	ImGui::SameLine();
	if (ImGui::Button("Rotation")) {
		gizmoMode = GizmoMode::Rotation;
	}
	ImGui::SameLine();
	if (ImGui::Button("Scale")) {
		gizmoMode = GizmoMode::Scale;
	}
	ImGui::SameLine();
	if (ImGui::Button("Display Colliders")) {
		showColliders = !showColliders;
		RenderCommand::ShowColliders(showColliders);
	}
}

bool BtnSqd::GuiLayer::OnSelectGameObject(OnSelectGameObjectEvent* e) {
	selectedObject = e->GetSelectedObject();
	return false;
}

bool BtnSqd::GuiLayer::OnDesroyGameObject(OnDesroyGameObjectEvent* e) {
	selectedObject = GameObject();
	return false;
}
