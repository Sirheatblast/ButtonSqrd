#include "RenderLayer.h"
namespace BtnSqd {
	BtnSqd::RenderLayer::RenderLayer(std::shared_ptr<BtnScene>& scene, std::shared_ptr<FrameBuffer> editorViewport, std::tuple<CameraComponent*, TransformComponent*> camera) :currentScene(scene), editorViewport(editorViewport) {
		auto& [camComp, transComp] = camera;
		editorCam = camComp;
		editorCamTransform = transComp;
		
		auto [width, height] = editorViewport->GetResolution();
		mousePickerBuffer.reset(FrameBuffer::Create(FrameBufferAttatchment::ColorAttatchment, width, height));
		TextureSettings texSettings;
		texSettings.texFormat = TexFormat::RGB_INT;
		mousePickerBuffer->AttatchTexture(TextureSlot::Albedo, texSettings);
		mousePickerBuffer->AttatchRenderBuffer();
		mousePickerBuffer->UnBind();

		mousePickerShader.reset(Shader::Create("./Assets/Shaders/MousePickerShader.vrt", "./Assets/Shaders/MousePickerShader.frag"));
	}

	BtnSqd::RenderLayer::~RenderLayer() {

	}

	void BtnSqd::RenderLayer::OnAttach() {

	}

	void BtnSqd::RenderLayer::OnDetach() {

	}

	void BtnSqd::RenderLayer::OnUpdate() {
		BtnSqd::RenderCommand::SetCurrentCamera(std::make_tuple(editorCam,editorCamTransform));
		editorCam->viewMatrix = glm::lookAt(editorCamTransform->transform.GetPosition(), editorCamTransform->transform.GetPosition() + -editorCamTransform->transform.front, editorCamTransform->transform.up);

		if (!guiLayer) {
			auto res = editorViewport->GetResolution();
			glm::vec2 viewPortSize = glm::vec2(res.first, res.second);
			guiLayer.reset(new BtnGuiLayer(currentScene, viewPortSize));
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Editor Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (!Application::GetApp()->GetWindow()->GetCursorLock()&&ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemActive()) {
			Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::GameEditorWindow));
		}

		if (windowSelected && canMousePick 
			&&ImGui::IsWindowHovered() && !ImGuizmo::IsOver() 
			&& !ImGuizmo::IsUsing() && !ImGui::IsAnyItemActive()) {
			SelectObject();
		}


		auto [windowWidth, windowHeight] = ImGui::GetWindowSize();
		editorViewport->SetResolution((int)windowWidth, (int)windowHeight);
		if (windowWidth!=lastWindowX||windowHeight!=lastWindowY) {
			lastWindowX = windowWidth;
			lastWindowY = windowHeight;
			editorCam->aspectRatio = windowWidth / windowHeight;
			editorCam->projectionMatrix = glm::perspective(glm::radians(editorCam->fov), editorCam->aspectRatio, editorCam->nearPlain, editorCam->farPlain);
		}

		RenderCommand::EnableBackFaceCulling();
		RenderCommand::DisableReflections();
		RenderCommand::DisableShadows();

		BtnSqd::Renderer::BeginScene(currentScene);

		editorViewport->Bind();
		BtnSqd::RenderCommand::DrawSkyBox();
		BtnSqd::RenderCommand::DrawScene();
		BtnSqd::Renderer::EndScene();
		editorViewport->UnBind();

		BtnSqd::RenderCommand::Clear();
		auto [viewPortW, viewPortH] = editorViewport->GetResolution();
		ImVec2 windowSize = ImVec2((float)viewPortW, (float)viewPortH);
		ImVec2 windowPos = ImGui::GetWindowPos();
		//Render To viewport image
		ImGui::Image(editorViewport->GetTexture()->GetId(), windowSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

		//Drag and drop
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_INFO")) {
				AssetPayloadType assetInfo = *(const AssetPayloadType*)payload->Data;
				HandleCreateModel(assetInfo);
				HandleCreateSuperGameObj(assetInfo);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void RenderLayer::HandleCreateSuperGameObj(BtnSqd::AssetPayloadType& assetInfo)	{
		if (assetInfo.assetType == AssetRawType::SuperGameObjectRaw) {
			std::shared_ptr<SuperGameObject> gTemplate;
			if (ResourceManager::GetLoadedSuperGameObjects().contains(assetInfo.path)) {
				gTemplate = ResourceManager::GetLoadedSuperGameObjects()[assetInfo.path];
			}
			else {
				ResourceManager::AddSuperGameObject(assetInfo.path, SuperGameObject::LoadSuperGameObject(assetInfo.path));
				gTemplate = ResourceManager::GetLoadedSuperGameObjects()[assetInfo.path];
			}
			GameObject gameAsset = currentScene->CreateNewGameObject(gTemplate.get());
			glm::vec3 spawnPos = (editorCamTransform->transform.GetPosition() + (-editorCamTransform->transform.front * 5.0f));
			gameAsset.GetComponent<TransformComponent>().transform.MoveTo(spawnPos);
		}
	}

	void RenderLayer::HandleCreateModel(BtnSqd::AssetPayloadType& assetInfo) {
		if (assetInfo.assetType == AssetRawType::ModelRaw) {
			GameObject gameAsset = currentScene->CreateNewGameObject(assetInfo.name);
			glm::vec3 spawnPos = (editorCamTransform->transform.GetPosition() + (-editorCamTransform->transform.front * 5.0f));
			gameAsset.GetComponent<TransformComponent>().transform.MoveTo(spawnPos);
			gameAsset.AddComponent<ModelComponent>();

			ModelComponent& modelComp = gameAsset.GetComponent<ModelComponent>();
			if (ResourceManager::GetLoadedModels().contains(assetInfo.path)) {
				modelComp.currentModel = ResourceManager::GetLoadedModels()[assetInfo.path];
			}
			else {
				ResourceManager::AddModel(assetInfo.path, Model(assetInfo.path, Shader::Create("./Assets/Shaders/PbrShader.vrt", "./Assets/Shaders/PbrShader.frag")));
				modelComp.currentModel = ResourceManager::GetLoadedModels()[assetInfo.path];
			}
			Application::GetApp()->PushEvent(new OnSelectGameObjectEvent(gameAsset));
		}
	}

	void BtnSqd::RenderLayer::OnEvent(Event* e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<BtnSqd::OnSelectWindowEvent>(std::bind(&RenderLayer::OnSelectWindow, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnEnableGuiEvent>([this](OnEnableGuiEvent* e)->bool{
			canMousePick = false;
			return false;
		});
		dispatcher.Dispatch<BtnSqd::OnDisableGuiEvent>([this](OnDisableGuiEvent* e)->bool{
			canMousePick = true;
			return false;
		});
	}
	bool BtnSqd::RenderLayer::OnSelectWindow(OnSelectWindowEvent* e) {
		if (e->GetWindow()==ActiveWindow::GameEditorWindow) {
			windowSelected = true;
		}
		else {
			windowSelected = false;
		}

		return true;
	}

	void BtnSqd::RenderLayer::SelectObject() {

		if (Input::IsMouseButtonPressed(MouseCode::Left)) {
			auto [mouseX, mouseY] = Input::GetMousePosition();
			auto [frameX, frameY] = mousePickerBuffer->GetResolution();

			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();

			float localX = mouseX - windowPos.x;
			float localY = mouseY - windowPos.y;

			// Clamp to image bounds
			localX = std::clamp(localX, 0.0f, windowSize.x - 1.0f);
			localY = std::clamp(localY, 0.0f, windowSize.y - 1.0f);

			int fbX = static_cast<int>(localX * (frameX / windowSize.x));
			int fbY = static_cast<int>(localY * (frameY / windowSize.y));

			// Clamp to framebuffer bounds
			fbX = std::clamp(fbX, 0, frameX - 1);
			fbY = std::clamp(fbY, 0, frameY - 1);

			// Flip Y for OpenGL
			fbY = frameY - fbY - 1;

			mousePickerBuffer->WBind();
			RenderCommand::DrawScene(mousePickerShader);
			auto [gameObjId, drawId, primitiveId] = mousePickerBuffer->GetPixelInformation(fbX, fbY);
			if (fbX > 1 && fbY > 1 && fbX < frameX - 1 && fbY < frameY - 1) {
				if (drawId == 0) {
					GameObject nullGameObj;
					Application::GetApp()->PushEvent(new OnSelectGameObjectEvent(nullGameObj));
				}
				else {
					Application::GetApp()->PushEvent(new OnSelectGameObjectEvent(*currentScene->GetgameObjects()[gameObjId]));
				}
			}
			mousePickerBuffer->UnBind();
		}
	}
}
