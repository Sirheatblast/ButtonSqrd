#include"EditorWindow.h"
#include<imgui_internal.h>

namespace BtnSqd {
	EditorWindow::EditorWindow(std::shared_ptr<BtnScene>& currentScene) :viewportAspectRatio(16.0f / 9.0f), editorCam(editorCamTransform), currentScene(currentScene), bGuiLayer(nullptr) {}
	void EditorWindow::Init() {
		auto [width, height] = Application::GetApp()->GetWindow()->GetResolution();
		editorViewport.reset(FrameBuffer::Create(FrameBufferAttatchment::ColorAttatchment, width, height));
		editorViewport->AttatchTexture(TextureSlot::Albedo);
		editorViewport->AttatchRenderBuffer();
		editorViewport->UnBind();

		bGuiLayer = new BtnGuiLayer(currentScene, { width,height }, "EditorGui");
		bGuiLayer->SetFrameBuffer(editorViewport);
		bGuiLayer->SetEnabled(false);

		editorCamTransform.transform.MoveTo(glm::vec3(0.0f, 0.0f, -3.0f));

		mousePickerShader.reset(Shader::Create("./Assets/Shaders/MousePickerShader.vrt", "./Assets/Shaders/MousePickerShader.frag"));
		renderLayers.PushLayer(new RenderLayer(currentScene, editorViewport, std::make_tuple(&editorCam, &editorCamTransform)));
		renderLayers.PushLayer(bGuiLayer);
		renderLayers.PushOverlay(new GuiLayer(std::make_tuple(&editorCam, &editorCamTransform)));
	}
	void EditorWindow::RenderEditor(GameObject& selectedObject) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Editor Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		windowPos = ImGui::GetCursorScreenPos();
		auto [width, height] = ImGui::GetWindowSize();
		auto [offsetX, offsetY] = ImGui::GetCursorScreenPos();
		windowSize = ImVec2(width,height);
		bGuiLayer->SetViewPortSize({ width,height });
		bGuiLayer->SetViewPortOffset({ offsetX,offsetY });


		for (auto layer : renderLayers) {
			if (layer->Enabled()) {
				layer->OnUpdate();
			}
		}

		HandleBtnGui();
		ScaleWidget();
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorWindow::WindowInput() {
		if (Input::IsMouseButtonUp(MouseCode::Right)) {
			firstClick = true;
		}
		if (Input::IsMouseButtonPressed(MouseCode::Right)) {

			BtnSqd::Input::SetCursorLock(true);
			auto [mousePositionX, mousePositionY] = BtnSqd::Input::GetMousePosition();

			float xMouseOffset = mousePositionX - lastMouseX;
			float yMouseOffset = lastMouseY - mousePositionY;
			lastMouseX = mousePositionX;
			lastMouseY = mousePositionY;

			if (!firstClick) {
				xMouseOffset *= 0.1f;//mouse sensitivity
				yMouseOffset *= 0.1f;

				yaw += xMouseOffset;
				pitch += yMouseOffset;

				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;

				glm::vec3 direction;
				direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
				direction.y = sin(glm::radians(pitch));
				direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

				editorCamTransform.transform.front = -glm::normalize(direction);
			}

			glm::vec3 nDirection = glm::vec3();
			glm::vec3 right = editorCamTransform.transform.GetRight();

			if (BtnSqd::Input::IsKeyPressed(KeyCode::W))
				nDirection -= editorCamTransform.transform.front;
			if (BtnSqd::Input::IsKeyPressed(KeyCode::S))
				nDirection += editorCamTransform.transform.front;
			if (BtnSqd::Input::IsKeyPressed(KeyCode::A))
				nDirection += right;
			if (BtnSqd::Input::IsKeyPressed(KeyCode::D))
				nDirection -= right;
			if (BtnSqd::Input::IsKeyPressed(KeyCode::Space)) {
				nDirection += editorCamTransform.transform.up;
			}
			if (BtnSqd::Input::IsKeyPressed(KeyCode::LeftControl)) {
				nDirection -= editorCamTransform.transform.up;
			}

			glm::vec3 nPos = editorCamTransform.transform.GetPosition() += (nDirection * BtnTime::GetDeltaTime() * 10.0f);
			editorCamTransform.transform.MoveTo(nPos);
			firstClick = false;
		}
		else {
			Input::SetCursorLock(false);
			EditorWindowHotKeys();
		}
	}
	void EditorWindow::SendEvent(Event* e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<OnSelectWidgetEvent>([this](OnSelectWidgetEvent* e)->bool {
			selectedWidget = e->GetWidget();
			return true;
												 });

		for (auto layer : renderLayers)
			layer->OnEvent(e);
	}
	void EditorWindow::EditorWindowHotKeys() {
		if (Input::IsKeyPressed(KeyCode::LeftShift) && Input::IsKeyPressed(KeyCode::A)) {
			Application::GetApp()->PushEvent(new OnCreateGameObjectEvent());
		}
	}
	void EditorWindow::HandleBtnGui() {
		if (!bGuiLayer->Enabled()) {
			return;
		}
		if (Input::IsMouseButtonPressed(MouseCode::Left) && !canDrag && !isResizeWidget) {
			auto [sX, sY] = Input::GetMousePosition();
			glm::vec2 mousePos = { sX, sY };
			selectedWidget = bGuiLayer->PickWidget(mousePos);
			if (selectedWidget) {
				Application::GetApp()->PushEvent(new OnGetWidgetEvent(selectedWidget));
			}
		}
		DragWidget();
	}

	void EditorWindow::DragWidget() {
		auto [sX, sY] = Input::GetMousePosition();
		glm::vec2 mousePos = { sX, sY };

		float xOffset = mousePos.x - lastMouseX;
		float yOffset = mousePos.y - lastMouseY;

		lastMouseX = mousePos.x;
		lastMouseY = mousePos.y;
		if (Input::IsMouseButtonPressed(MouseCode::Left)) {
			if (selectedWidget) {
				if (selectedWidget->GetHovered() && !isResizeWidget) {
					canDrag = true;
				}
				if (canDrag) {
					glm::vec2 currentPos = selectedWidget->GetPos();
					glm::vec2 offset = glm::vec2(xOffset, yOffset);
					glm::vec2 nPos = currentPos + offset;
					glm::vec2 winSize = glm::vec2(windowSize.x, windowSize.y);
					selectedWidget->SetPos(nPos);

					float maxPercent = 100.0f;
					glm::vec2 nPercent = (nPos / (winSize-selectedWidget->GetDimensions())) * maxPercent;
					nPercent=glm::clamp(nPercent, glm::vec2(0.0f), glm::vec2(maxPercent));
					selectedWidget->SetPosPercent(nPercent);
				}
			}
		}
		if (Input::IsMouseButtonUp(MouseCode::Left)) {
			canDrag = false;
			isResizeWidget = false;
		}
	}

	void EditorWindow::ScaleWidget() {
		if (!selectedWidget) return;

		glm::vec2 winPos = glm::vec2(windowPos.x, windowPos.y);
		glm::vec2 pos = selectedWidget->GetPos();
		glm::vec2 size = selectedWidget->GetDimensions();
		glm::vec2 handlePos = winPos + pos + size;

		float handleRadius = 8.0f;
		ImVec2 mousePos = ImGui::GetIO().MousePos;
		ImGuiID handleId = ImGui::GetID("##ResizeHandle");

		float dx = mousePos.x - handlePos.x;
		float dy = mousePos.y - handlePos.y;
		bool isHovered = (dx * dx + dy * dy) <= (handleRadius * handleRadius);

		if (isHovered && ImGui::GetActiveID() != handleId) {
			isResizeWidget = true;
			if (ImGui::IsMouseClicked(0)) {
				ImGui::SetActiveID(handleId, ImGui::GetCurrentWindow());
			}
		}

		if (isResizeWidget) {
			if (ImGui::IsMouseDown(0)) {
				canDrag = false;
				ImVec2 delta = ImGui::GetIO().MouseDelta;
				selectedWidget->SetDimensions(size + glm::vec2(delta.x, delta.y));
			}
		}

		ImU32 color = (ImGui::GetActiveID() == handleId) ? IM_COL32(200, 200, 200, 255) :
			isHovered ? IM_COL32(255, 255, 255, 255) : IM_COL32(225, 225, 225, 255);

		ImGui::GetWindowDrawList()->AddCircleFilled(
			ImVec2(handlePos.x, handlePos.y),
			handleRadius,
			color
		);
	}

}
