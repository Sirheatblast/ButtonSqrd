#include "EditorInterface.h"

#include"ButtonSqrd/Core/SceneSerializer.h"

namespace BtnSqd {
	EditorInterface::EditorInterface() :currentScene(new BtnScene()), btnPhysicObj(currentScene), sceneDir(currentScene), propMenue(currentScene, btnPhysicObj), selectedWindow(ActiveWindow::MainGUI), gameWindow(currentScene), editorWindow(currentScene) {
		Application::GetApp()->GetWindow()->SetIcon("./EngineAssets/Textures/EditorIcon.png");

		RenderCommand::Init();
		btnPhysicObj.Init();

		ResourceManager::LoadAllSkyboxes();
		ResourceManager::LoadAllParticleTextures();
		ResourceManager::LoadAllShaders();
		ResourceManager::LoadAllModels();
		ResourceManager::LoadAllSuperGameObjects();
		ResourceManager::LoadAllSounds();

		ImGui::StyleColorsDark();
		std::filesystem::path path = std::filesystem::current_path();

		editorWindow.SetScene(currentScene);
		editorWindow.Init();

		RenderCommand::InitShadowBuffer();
		RenderCommand::InitReflectionsBuffer();
		RenderCommand::InitSkybox();

		SceneSerializer sceneSave(currentScene.get());
		sceneSave.Deserialize("./Assets/Scenes/ColliderTest.BtnScn");
		btnPhysicObj.OnSceneLoad();
		currentScene->SetPhysics(&btnPhysicObj);
		editorScene = currentScene;

		SetGuiStyle();
	}

	void EditorInterface::OnUpdate() {
		Application::GetApp()->GetWindow()->UpdateImGui();
		Renderer::BeginScene(currentScene);

		for (auto& [id, gameObj] : currentScene->GetgameObjects()) {
			if (gameObj->IsValid() && !gameObj->GetChildren().empty())
				gameObj->UpdateChildrenTransforms();
		}

		RenderCommand::ProcessLights();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		ImGui::DockSpaceOverViewport();

		EditorInput();

		RenderCommand::UpdateRenderer();
		editorWindow.RenderEditor(selectedObject);
		gameWindow.RenderWindow();

		if (runtimeState == RuntimeState::Playing && isCompiled) {
			//Scripts
			try {
				currentScene->OnRuntimeUpdate();
				currentScene->OnRuntimePhysicsUpdate();
				btnPhysicObj.Update();
				for (auto& [listener, transform] : currentScene->GetRegister().GetAllOf<AudioListenerComponent, TransformComponent>()) {
					listener.listener.SetPosition(transform.transform.GetPosition());
					listener.listener.SetVelocity(transform.transform.GetPosition());
					listener.listener.SetOrientation(-transform.transform.front, transform.transform.up);
					listener.listener.Update();
				}
				for (auto& [aSource, transform] : currentScene->GetRegister().GetAllOf<AudioSourceComponent, TransformComponent>()) {
					aSource.source.SetPosition(transform.transform.GetPosition());
					aSource.source.SetVelocity(transform.transform.GetPosition());
				}
			}
			catch (const std::runtime_error& e) {
				BTNLOG_ERROR(e.what())
					Application::GetApp()->PushEvent(new OnEndRuntimeEvent());
			}

		}

		assetDir.OnUpdate();
		sceneDir.OnUpdate(selectedObject);
		propMenue.OnUpdate(selectedObject);
		animController.OnUpdate(selectedObject);
		console.OnUpdate();

		CreateNewGameObjectWindow();

		ImGui::Render();
		BtnSqd::Application::GetApp()->GetWindow()->DrawImGuiFrame();
		BtnSqd::Renderer::EndScene();
	}

	void EditorInterface::SetGuiStyle() {
		ImGui::GetIO().Fonts->AddFontFromFileTTF("EngineAssets/Fonts/NotoSans-Regular.ttf", 18.0f);
		ImGuiStyle& style = ImGui::GetStyle();

		ImVec4 baseBackground = ImVec4(0.08f, 0.08f, 0.11f, 1.00f);
		ImVec4 baseDark = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
		ImVec4 surfaceLight = ImVec4(0.25f, 0.28f, 0.40f, 1.00f);
		ImVec4 surfaceDark = ImVec4(0.10f, 0.10f, 0.14f, 1.00f);
		ImVec4 accentPrimary = ImVec4(0.50f, 0.68f, 0.95f, 1.00f);
		ImVec4 textPrimary = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);

		auto accentWithAlpha = [&](float alpha) {
			return ImVec4(accentPrimary.x, accentPrimary.y, accentPrimary.z, alpha);
			};

		ImVec4* colors = style.Colors;

		colors[ImGuiCol_WindowBg] = baseBackground;
		colors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
		colors[ImGuiCol_PopupBg] = baseDark;
		colors[ImGuiCol_MenuBarBg] = baseDark;
		colors[ImGuiCol_DockingEmptyBg] = baseDark;

		colors[ImGuiCol_Text] = textPrimary;
		colors[ImGuiCol_TextDisabled] = ImVec4(textPrimary.x, textPrimary.y, textPrimary.z, 0.5f);

		colors[ImGuiCol_Border] = baseDark;
		colors[ImGuiCol_Separator] = baseDark;
		colors[ImGuiCol_SeparatorHovered] = accentPrimary;
		colors[ImGuiCol_SeparatorActive] = accentPrimary;

		colors[ImGuiCol_TitleBg] = baseDark;
		colors[ImGuiCol_TitleBgActive] = baseBackground;
		colors[ImGuiCol_Header] = surfaceLight;
		colors[ImGuiCol_HeaderHovered] = accentWithAlpha(0.4f);
		colors[ImGuiCol_HeaderActive] = accentPrimary;

		colors[ImGuiCol_FrameBg] = surfaceDark;
		colors[ImGuiCol_FrameBgHovered] = surfaceLight;
		colors[ImGuiCol_FrameBgActive] = accentWithAlpha(0.3f);

		colors[ImGuiCol_Button] = surfaceLight;
		colors[ImGuiCol_ButtonHovered] = accentWithAlpha(0.6f);
		colors[ImGuiCol_ButtonActive] = accentPrimary;

		colors[ImGuiCol_Tab] = surfaceDark;
		colors[ImGuiCol_TabHovered] = surfaceLight;
		colors[ImGuiCol_TabSelected] = baseDark;
		colors[ImGuiCol_TabSelectedOverline] = baseBackground;
		colors[ImGuiCol_TabDimmed] = surfaceDark;
		colors[ImGuiCol_TabDimmedSelected] = baseBackground;

		colors[ImGuiCol_CheckMark] = accentPrimary;
		colors[ImGuiCol_SliderGrab] = accentWithAlpha(0.7f);
		colors[ImGuiCol_SliderGrabActive] = accentPrimary;
		colors[ImGuiCol_DockingPreview] = accentWithAlpha(0.4f);
		colors[ImGuiCol_NavCursor] = accentPrimary;

		style.WindowRounding = 6.0f;
		style.ChildRounding = 6.0f;
		style.FrameRounding = 6.0f;
		style.PopupRounding = 6.0f;
		style.ScrollbarRounding = 6.0f;
		style.GrabRounding = 6.0f;
		style.TabRounding = 6.0f;

		style.FrameBorderSize = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.FramePadding = ImVec2(8, 4);
		style.ItemSpacing = ImVec2(8, 5);
	}


	void EditorInterface::EditorInput() {
		if (selectedWindow == ActiveWindow::GameEditorWindow) {
			editorWindow.WindowInput();
		}
		else {
			if (saveScenePossible && runtimeState == RuntimeState::End && Input::IsKeyPressed(KeyCode::LeftControl) && Input::IsKeyPressed(KeyCode::S)) {
				saveScenePossible = false;
				BTNLOG_INFO("Saving scene: {}", currentScene->GetName());
				SceneSerializer saveScene(currentScene.get());
				saveScene.Serialize(destination + currentScene->GetName() + ".BtnScn", currentScene->GetName());
			}
			if (!saveScenePossible && Input::IsKeyUp(KeyCode::LeftControl) || Input::IsKeyUp(KeyCode::S)) {
				saveScenePossible = true;
			}
		}
		if (Input::IsKeyPressed(KeyCode::LeftControl) && Input::IsKeyPressed(KeyCode::Grave)) {
			Application::GetApp()->GetWindow()->SetCursorLock(false);
		}
	}

	void EditorInterface::CreateNewGameObjectWindow() {
		if (showGameObjectCreateWindow) {
			ImGui::OpenPopup("Create A new GameObject");
			showGameObjectCreateWindow = false;
		}

		auto [camera, camTransform] = editorWindow.GetEditorCamera();
		glm::vec3 spawnPos = camTransform.transform.GetPosition() + (-camTransform.transform.front * 4.5f);

		if (ImGui::BeginPopup("Create A new GameObject", ImGuiPopupFlags_MouseButtonRight)) {
			ImGui::Text("Create GameObject");

			if (ImGui::Selectable("Widget Canvas")) {
				selectedObject = currentScene->CreateNewGameObject("New Widget Canvas");
				auto& wCanvas = selectedObject.AddComponent<WidgetCanvasComponent>();
				wCanvas.useWholeScreen = true;
			}

			if (ImGui::Selectable("Empty")) {
				selectedObject = currentScene->CreateNewGameObject("Empty GameObject");
			}
			if (ImGui::Selectable("Cube")) {
				GameObject nGobj = currentScene->CreateNewGameObject("New Cube");
				auto& modelComp = nGobj.AddComponent<ModelComponent>();
				modelComp.currentModel = ResourceManager::GetLoadedModels()["./Assets/Models/Cube.fbx"];
				auto& transformComp = nGobj.GetComponent<TransformComponent>();
				transformComp.transform.MoveTo(spawnPos);
				selectedObject = nGobj;
			}
			if (ImGui::Selectable("Sphere")) {
				GameObject nGobj = currentScene->CreateNewGameObject("New Sphere");
				auto& modelComp = nGobj.AddComponent<ModelComponent>();
				modelComp.currentModel = ResourceManager::GetLoadedModels()["./Assets/Models/Sphere.fbx"];
				auto& transformComp = nGobj.GetComponent<TransformComponent>();
				transformComp.transform.MoveTo(spawnPos);
				selectedObject = nGobj;
			}
			if (ImGui::Selectable("Capsule")) {
				GameObject nGobj = currentScene->CreateNewGameObject("New Capsule");
				auto& modelComp = nGobj.AddComponent<ModelComponent>();
				modelComp.currentModel = ResourceManager::GetLoadedModels()["./Assets/Models/Capsule.fbx"];
				auto& transformComp = nGobj.GetComponent<TransformComponent>();
				transformComp.transform.MoveTo(spawnPos);
				selectedObject = nGobj;
			}
			if (ImGui::Selectable("Plane")) {
				GameObject nGobj = currentScene->CreateNewGameObject("New Plane");
				auto& modelComp = nGobj.AddComponent<ModelComponent>();
				modelComp.currentModel = ResourceManager::GetLoadedModels()["./Assets/Models/Plane.fbx"];
				auto& transformComp = nGobj.GetComponent<TransformComponent>();
				transformComp.transform.MoveTo(spawnPos);
				selectedObject = nGobj;
			}
			ImGui::EndPopup();
		}
	}

	void EditorInterface::OnAttach() {

	}

	void EditorInterface::OnDetach() {
		ImGui::DestroyContext();
		btnPhysicObj.ShutDown();
	}

	void EditorInterface::OnEvent(BtnSqd::Event* e) {
		BtnSqd::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<BtnSqd::OnMouseMoveEvent>(std::bind(&EditorInterface::OnMouseMoveEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnMouseClickEvent>(std::bind(&EditorInterface::OnMouseClickEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnMouseClickReleaseEvent>(std::bind(&EditorInterface::OnMouseClickReleaseEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnMouseScrollEvent>(std::bind(&EditorInterface::OnMouseScrollEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnKeyDownEvent>(std::bind(&EditorInterface::OnKeyDownEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnKeyReleaseEvent>(std::bind(&EditorInterface::OnKeyReleaseEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnWindowResizeEvent>(std::bind(&EditorInterface::OnWindowResizeEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnKeyTypedEvent>(std::bind(&EditorInterface::OnKeyTypedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnSelectGameObjectEvent>(std::bind(&EditorInterface::OnSelectGameObject, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnSelectWindowEvent>(std::bind(&EditorInterface::OnSelectWindow, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnCreateGameObjectEvent>(std::bind(&EditorInterface::OnCreateGameObject, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnDesroyGameObjectEvent>(std::bind(&EditorInterface::OnDestroyGameObject, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnSetActiveCameraEvent>(std::bind(&EditorInterface::OnSelectCamera, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnStartRuntimeEvent>(std::bind(&EditorInterface::OnStartRuntime, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnPauseRuntimeEvent>(std::bind(&EditorInterface::OnPauseRuntime, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnEndRuntimeEvent>(std::bind(&EditorInterface::OnEndRuntime, this, std::placeholders::_1));
		dispatcher.Dispatch<BtnSqd::OnChangeSceneEvent>(std::bind(&EditorInterface::OnChangeScene, this, std::placeholders::_1));

		editorWindow.SendEvent(e);
		gameWindow.SendEvent(e);
		propMenue.SendEvent(e);
	}

	bool EditorInterface::OnMouseMoveEvent(BtnSqd::OnMouseMoveEvent* e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e->GetPosition().first, e->GetPosition().second);

		return false;
	}

	bool EditorInterface::OnMouseClickEvent(BtnSqd::OnMouseClickEvent* e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(e->GetButton(), true);

		return false;
	}

	bool EditorInterface::OnMouseClickReleaseEvent(BtnSqd::OnMouseClickReleaseEvent* e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(e->GetButton(), false);

		return false;
	}

	bool EditorInterface::OnMouseScrollEvent(BtnSqd::OnMouseScrollEvent* e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH = e->GetOffset().first;
		io.MouseWheel = e->GetOffset().second;

		return false;
	}

	bool EditorInterface::OnKeyDownEvent(BtnSqd::OnKeyDownEvent* e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(ConvertToImguiKey(e->GetKey()), true);

		return false;
	}

	bool EditorInterface::OnKeyReleaseEvent(BtnSqd::OnKeyReleaseEvent* e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(ConvertToImguiKey(e->GetKey()), false);

		return false;
	}

	bool EditorInterface::OnWindowResizeEvent(BtnSqd::OnWindowResizeEvent* e) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)e->GetWidth(), (float)e->GethHeight());
		io.DisplayFramebufferScale = ImVec2(1.0, 1.0);

		return false;
	}

	bool EditorInterface::OnKeyTypedEvent(BtnSqd::OnKeyTypedEvent* e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(e->GetKey());

		return false;
	}

	bool EditorInterface::OnSelectGameObject(OnSelectGameObjectEvent* e) {
		selectedObject = e->GetSelectedObject();
		return false;
	}

	bool EditorInterface::OnSelectWindow(OnSelectWindowEvent* e) {
		selectedWindow = e->GetWindow();
		return false;
	}

	bool EditorInterface::OnCreateGameObject(OnCreateGameObjectEvent* e) {
		showGameObjectCreateWindow = true;
		return false;
	}

	bool EditorInterface::OnDestroyGameObject(OnDesroyGameObjectEvent* e) {
		currentScene->DeleteGameObject(&e->GetGameObject());
		selectedObject = GameObject();
		return false;
	}

	bool EditorInterface::OnSelectCamera(OnSetActiveCameraEvent* e) {
		GameObject currentCam = currentScene->GetActiveCamera();
		GameObject newCam = e->GetCamera();
		if (!currentCam.IsValid() || e->GetCamera().GetId() != currentCam.GetId()) {
			currentScene->SetActiveCamera(newCam.GetId());
		}

		return false;
	}

	bool EditorInterface::OnChangeScene(OnChangeSceneEvent* e) {
		selectedObject = GameObject();
		currentScene->UnLoadScene();
		*currentScene = *e->GetScene();
		editorScene = currentScene;

		return false;
	}

	bool EditorInterface::OnStartRuntime(OnStartRuntimeEvent* e) {
		if (runtimeState == RuntimeState::End) {
			CompileScripts();

			for (auto& [particleEmitter] : currentScene->GetRegister().GetAllOfRef<ParticleEmitterComponent>()) {
				particleEmitter.get().isEmiting = false;
			}

			currentScene = BtnScene::CopyToShared(editorScene);
			selectedObject = GameObject();
			Application::GetApp()->PushEvent(new OnSelectGameObjectEvent(selectedObject));

			for (auto& [gId, scriptComp] : currentScene->GetRegister().GetAllOfID<ScriptComponent>()) {
				for (auto& script : scriptComp.scripts) {
					script.script->SetGameObject(currentScene->GetgameObjects()[gId].get());
					script.script->SetScene(currentScene.get());

					uint32_t count;
					auto* varArray = script.script->GetEditables(count);
					for (uint32_t i = 0; i < count; i++) {
						if (varArray[i].type == DataType::GameObject) {
							auto gameObj = static_cast<GameObject*>(varArray[i].data);
							uint32_t id = gameObj->GetId();
							*gameObj = *currentScene->GetgameObjects()[id];
						}
					}
					script.script->OnAwake();
				}
			}

			for (auto& [boneComp] : currentScene->GetRegister().GetAllOfRef<BoneComponent>()) {
				auto& bone = boneComp.get();
				if (currentScene->GetgameObjects()[bone.armatureId]->CheckObjectForComponent<ArmatureComponent>()) {
					auto& armature = currentScene->GetgameObjects()[bone.armatureId]->GetComponent<ArmatureComponent>().armature;
					bone.selectedBone = &armature.GetBones()[armature.GetBoneIds().at(bone.boneName)];
				}
			}

			currentScene->OnRuntimeStart();
			selectedObject = GameObject();
		}
		runtimeState = RuntimeState::Playing;
		return false;
	}

	bool EditorInterface::OnPauseRuntime(OnPauseRuntimeEvent* e) {
		if (runtimeState == RuntimeState::Playing) {
			runtimeState = RuntimeState::Pause;
		}
		return false;
	}

	bool EditorInterface::OnEndRuntime(OnEndRuntimeEvent* e) {
		btnPhysicObj.Reset();
		currentScene = editorScene;
		btnPhysicObj.ReInit();

		selectedObject = GameObject();
		Application::GetApp()->PushEvent(new OnSelectGameObjectEvent(selectedObject));
		runtimeState = RuntimeState::End;
		isCompiled = false;
		return false;
	}

	void EditorInterface::CompileScripts() {
		std::thread([this]() {
			isCompiled = ScriptManager::CompileScripts();
		}).detach();
	}

	ImGuiKey BtnSqd::EditorInterface::ConvertToImguiKey(KeyCode keycode) {
		switch (keycode) {
		case KeyCode::Space: return ImGuiKey_Space;
		case KeyCode::Enter: return ImGuiKey_Enter;
		case KeyCode::EscapeKey: return ImGuiKey_Escape;
		case KeyCode::Tab: return ImGuiKey_Tab;
		case KeyCode::BackSpace: return ImGuiKey_Backspace;
		case KeyCode::Insert: return ImGuiKey_Insert;
		case KeyCode::Delete: return ImGuiKey_Delete;
		case KeyCode::Home: return ImGuiKey_Home;
		case KeyCode::End: return ImGuiKey_End;
		case KeyCode::PageUp: return ImGuiKey_PageUp;
		case KeyCode::PageDown: return ImGuiKey_PageDown;
		case KeyCode::Left: return ImGuiKey_LeftArrow;
		case KeyCode::Right: return ImGuiKey_RightArrow;
		case KeyCode::Up: return ImGuiKey_UpArrow;
		case KeyCode::Down: return ImGuiKey_DownArrow;

			// Function keys
		case KeyCode::F1: return ImGuiKey_F1;
		case KeyCode::F2: return ImGuiKey_F2;
		case KeyCode::F3: return ImGuiKey_F3;
		case KeyCode::F4: return ImGuiKey_F4;
		case KeyCode::F5: return ImGuiKey_F5;
		case KeyCode::F6: return ImGuiKey_F6;
		case KeyCode::F7: return ImGuiKey_F7;
		case KeyCode::F8: return ImGuiKey_F8;
		case KeyCode::F9: return ImGuiKey_F9;
		case KeyCode::F10: return ImGuiKey_F10;
		case KeyCode::F11: return ImGuiKey_F11;
		case KeyCode::F12: return ImGuiKey_F12;

			// Modifier keys
		case KeyCode::LeftShift: return ImGuiKey_LeftShift;
		case KeyCode::RightShift: return ImGuiKey_RightShift;
		case KeyCode::LeftControl: return ImGuiKey_LeftCtrl;
		case KeyCode::RightControl: return ImGuiKey_RightCtrl;
		case KeyCode::LeftAlt: return ImGuiKey_LeftAlt;
		case KeyCode::RightAlt: return ImGuiKey_RightAlt;
		case KeyCode::LeftSuper: return ImGuiKey_LeftSuper;
		case KeyCode::RightSuper: return ImGuiKey_RightSuper;

			// Alphanumeric keys
		case KeyCode::A: return ImGuiKey_A;
		case KeyCode::B: return ImGuiKey_B;
		case KeyCode::C: return ImGuiKey_C;
		case KeyCode::D: return ImGuiKey_D;
		case KeyCode::E: return ImGuiKey_E;
		case KeyCode::F: return ImGuiKey_F;
		case KeyCode::G: return ImGuiKey_G;
		case KeyCode::H: return ImGuiKey_H;
		case KeyCode::I: return ImGuiKey_I;
		case KeyCode::J: return ImGuiKey_J;
		case KeyCode::K: return ImGuiKey_K;
		case KeyCode::L: return ImGuiKey_L;
		case KeyCode::M: return ImGuiKey_M;
		case KeyCode::N: return ImGuiKey_N;
		case KeyCode::O: return ImGuiKey_O;
		case KeyCode::P: return ImGuiKey_P;
		case KeyCode::Q: return ImGuiKey_Q;
		case KeyCode::R: return ImGuiKey_R;
		case KeyCode::S: return ImGuiKey_S;
		case KeyCode::T: return ImGuiKey_T;
		case KeyCode::U: return ImGuiKey_U;
		case KeyCode::V: return ImGuiKey_V;
		case KeyCode::W: return ImGuiKey_W;
		case KeyCode::X: return ImGuiKey_X;
		case KeyCode::Y: return ImGuiKey_Y;
		case KeyCode::Z: return ImGuiKey_Z;

		case KeyCode::KeyPad0: return ImGuiKey_Keypad0;
		case KeyCode::KeyPad1: return ImGuiKey_Keypad1;
		case KeyCode::KeyPad2: return ImGuiKey_Keypad2;
		case KeyCode::KeyPad3: return ImGuiKey_Keypad3;
		case KeyCode::KeyPad4: return ImGuiKey_Keypad4;
		case KeyCode::KeyPad5: return ImGuiKey_Keypad5;
		case KeyCode::KeyPad6: return ImGuiKey_Keypad6;
		case KeyCode::KeyPad7: return ImGuiKey_Keypad7;
		case KeyCode::KeyPad8: return ImGuiKey_Keypad8;
		case KeyCode::KeyPad9: return ImGuiKey_Keypad9;
		case KeyCode::KeyPadDecimal: return ImGuiKey_KeypadDecimal;
		case KeyCode::KeyPadDivide: return ImGuiKey_KeypadDivide;
		case KeyCode::KeyPadMultiply: return ImGuiKey_KeypadMultiply;
		case KeyCode::KeyPadSubtract: return ImGuiKey_KeypadSubtract;
		case KeyCode::KeyPadAdd: return ImGuiKey_KeypadAdd;
		case KeyCode::KeyPadEnter: return ImGuiKey_KeypadEnter;
		case KeyCode::KeyPadEqual: return ImGuiKey_KeypadEqual;


		default: return ImGuiKey_None; // Return None for unmapped keys
		}
	}

}
