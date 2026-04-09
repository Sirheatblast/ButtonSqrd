#include "GameLayer.h"

namespace BtnSqd {
	GameLayer::GameLayer(std::shared_ptr<BtnScene>& scene, std::shared_ptr<FrameBuffer> gameViewport):currentScene(scene),gameViewport(gameViewport),currentState(RuntimeState::End) {
		activeCamera = currentScene->GetActiveCamera();
		
	}
	GameLayer::~GameLayer() {

	}
	void GameLayer::OnAttach() {

	}
	void GameLayer::OnDetach() {

	}
	void GameLayer::OnUpdate() {

		activeCamera = currentScene->GetActiveCamera();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (activeCamera.IsValid()&&activeCamera.CheckObjectForComponent<CameraComponent>()) {
			CameraComponent& gameCam = activeCamera.GetComponent<CameraComponent>();
			TransformComponent camTransform = activeCamera.GetComponent<TransformComponent>();
			BtnSqd::RenderCommand::SetCurrentCamera(std::make_tuple(&gameCam, &camTransform));
			gameCam.viewMatrix = glm::lookAt(camTransform.transform.GetPosition(), camTransform.transform.GetPosition() + -camTransform.transform.front, camTransform.transform.up);

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemActive()) {
				Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::GameViewportWindow));
			}


			auto [windowWidth, windowHeight] = ImGui::GetContentRegionAvail();
			gameViewport->SetResolution((int)windowWidth, (int)windowHeight);
			if (windowWidth != lastWindowX || windowHeight != lastWindowY) {
				lastWindowX = windowWidth;
				lastWindowY = windowHeight;
				gameCam.aspectRatio = windowWidth / windowHeight;
				gameCam.projectionMatrix = glm::perspective(glm::radians(gameCam.fov), gameCam.aspectRatio, gameCam.nearPlain, gameCam.farPlain);
			}

			if (!guiLayer) {
				guiLayer.reset(new BtnGuiLayer(currentScene,{windowWidth,windowHeight}));
			}
			guiLayer->SetViewPortSize({ windowWidth,windowHeight });

			auto [windowPosX, windowPosY] = ImGui::GetWindowPos();
			Application::GetApp()->SetViewPort({glm::vec2(windowWidth,windowHeight),glm::vec2(windowPosX,windowPosY+imageOffset)});

			BtnSqd::Renderer::BeginScene(currentScene);

			BtnSqd::RenderCommand::EnableBackFaceCulling();
			BtnSqd::RenderCommand::EnableShadows();
			BtnSqd::RenderCommand::EnableReflections();

			gameViewport->Bind();

			BtnSqd::RenderCommand::DrawSkyBox();
			BtnSqd::RenderCommand::DrawScene();
			BtnSqd::Renderer::EndScene();

			guiLayer->OnUpdate();

			gameViewport->UnBind();

			BtnSqd::RenderCommand::Clear();
			
			auto [viewPortW, viewPortH] = gameViewport->GetResolution();
			ImVec2 windowSize = ImVec2((float)viewPortW, (float)viewPortH);
			
			ImVec2 topLeft = ImGui::GetCursorScreenPos();
			ImVec2 bottom_right = ImVec2(topLeft.x + windowSize.x, topLeft.y + imageOffset);
			unsigned int grayColor = 45;
			ImU32 color = IM_COL32(grayColor, grayColor, grayColor, 255);

			ImGui::GetWindowDrawList()->AddRectFilled(topLeft, bottom_right, color);

			//Render To viewport image
			ImGui::SetCursorPosY(imageOffset);
			ImGui::Image(gameViewport->GetTexture()->GetId(), windowSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		}
		float xPos = (ImGui::GetWindowSize().x / 2.0f) - 75.0f;
		float yPos = 25.0f;
		ImVec2 buttonPos = ImVec2(xPos, yPos);

		ImGui::SetCursorPos(buttonPos);
		if (ImGui::Button("Pause") && currentState == RuntimeState::Playing) {
			Application::GetApp()->PushEvent(new OnPauseRuntimeEvent());
			currentState = RuntimeState::Pause;
		}
		ImGui::SameLine();
		if (currentState != RuntimeState::Playing) {
			if (ImGui::Button("Play")) {
				Application::GetApp()->PushEvent(new OnStartRuntimeEvent());
				currentState = RuntimeState::Playing;
			}
		}
		else {
			if (ImGui::Button("End")) {
				Application::GetApp()->PushEvent(new OnEndRuntimeEvent());
				currentState = RuntimeState::End;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}
	void GameLayer::OnEvent(Event* e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<BtnSqd::OnSetActiveCameraEvent>(std::bind(&GameLayer::OnSelectCamera, this, std::placeholders::_1));
	}
	bool GameLayer::OnSelectCamera(OnSetActiveCameraEvent* e)	{
		activeCamera = currentScene->GetActiveCamera();
		return false;
	}
}
