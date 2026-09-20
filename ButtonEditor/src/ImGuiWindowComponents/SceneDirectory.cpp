#include "SceneDirectory.h"
#include"StaticPopups.h"

namespace BtnSqd {
	SceneDirectory::SceneDirectory(std::shared_ptr<BtnScene>& currentScene) :currentScene(currentScene) {
		hiddenChildrenTexture.reset(Texture::Create("./EngineAssets/Textures/HideChildrenArrow.png", TextureSettings()));
		showChildrenTexture.reset(Texture::Create("./EngineAssets/Textures/ShowChildrenArrow.png", TextureSettings()));
	}
	void SceneDirectory::OnUpdate(GameObject& selectedObj) {
		if (!selectedObj.IsValid()) {
			GameObject temp = GameObject();
			Application::GetApp()->PushEvent(new OnSelectGameObjectEvent(temp));
		}

		ImGui::Begin("Scene directory");

		ImGui::BeginChild("SceneTargetDropChild");

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemActive()) {
			Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::MainGUI));
		}
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !ImGui::IsAnyItemHovered()) {
			Application::GetApp()->PushEvent(new OnCreateGameObjectEvent());
		}

		if (ImGui::Selectable(currentScene->GetName().c_str())) {
			sceneConfigMenue = !sceneConfigMenue;
		}

		if (ImGui::Button("Save Scene")) {
			saveScene = true;
			strcpy_s(buffer, currentScene->GetName().c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button("New Scene ##CreatNewSceneButton")) {
			Application::GetApp()->PushEvent(new OnChangeSceneEvent(new BtnScene()));
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Scene")) {
			loadScene = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Create new GameObject")) {
			Application::GetApp()->PushEvent(new OnCreateGameObjectEvent());
		}

		if (saveScene) {
			SaveScene();
		}
		if (loadScene) {
			LoadScene();
		}
		if (sceneConfigMenue) {
			SceneConfigMenue();
		}

		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		for (auto& [id, gameObj] : currentScene->GetgameObjects()) {
			if (!gameObj->IsValid() || !gameObj->CheckObjectForComponent<TagComponenet>()) {
				continue;
			}
			if (!gameObj->IsChild()) {
				if (selectedObj.IsValid() && gameObj->GetUUID() == selectedObj.GetUUID()) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				}

				DisplayGameObject(gameObj, selectedObj);

				if (gameObj->GetShowChildren()) {
					HandleDisplayWidgets(gameObj, selectedObj);
					DisplayChildren(*gameObj, selectedObj);
				}
				ImGui::PopStyleColor();
			}
		}

		if (ImGui::BeginPopup("GameObjectOptionsPopUp")) {
			ImGui::Text("GameObject Options");
			if (ImGui::MenuItem("Duplicate")) {
				GameObject dup = currentScene->DuplicateGameObject(&selectedObj);
			}
			if (ImGui::MenuItem("Delete")) {
				DestroySelected(selectedObj);
			}
			ImGui::EndPopup();
		}

		if (Input::IsKeyPressed(KeyCode::Delete)) {
			DestroySelected(selectedObj);
		}

		ImGui::PopStyleColor(1);
		ImGui::EndChild();
		DropAddChild(nullptr);

		ImGui::End();
	}
	void SceneDirectory::DisplayGameObject(std::shared_ptr<BtnSqd::GameObject>& gameObj, GameObject& selectedObj) {
		ImGui::BeginGroup();

		if (ImGui::Selectable(gameObj->GetComponent<TagComponenet>().tag.c_str())) {
			Application::GetApp()->PushEvent(new OnSelectGameObjectEvent(*gameObj));
			Application::GetApp()->PushEvent(new OnDisableGuiEvent("EditorGui"));
			Application::GetApp()->PushEvent(new OnEditWidgetDisableEvent("EditorGui"));
		}


		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			gameObj->ChangeShowChildren();
		}
		DropAddChild(gameObj);
		DragGameObj(gameObj.get());

		if (!gameObj->GetChildren().empty() || gameObj->CheckObjectForComponent<WidgetCanvasComponent>()) {
			unsigned int texId = (gameObj->GetShowChildren()) ? showChildrenTexture->GetId() : hiddenChildrenTexture->GetId();

			ImGui::SameLine();
			ImGui::SetNextItemAllowOverlap();
			ImGui::Image(texId, ImVec2(childrenTexSize, childrenTexSize));
		}

		ImGui::EndGroup();

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("GameObjectOptionsPopUp");
			selectedObj = *gameObj;
		}
	}
	void SceneDirectory::HandleDisplayWidgets(std::shared_ptr<BtnSqd::GameObject>& gameObj, BtnSqd::GameObject& selectedObj) {
		if (gameObj->CheckObjectForComponent<WidgetCanvasComponent>()) {
			WidgetCanvasComponent& widgetComp = gameObj->GetComponent<WidgetCanvasComponent>();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			ImGui::Indent(20.0f);
			ImGui::Selectable(("Widgets##ShowBtnWidgetsSelectableForObj_" + std::to_string(gameObj->GetUUID())).c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				widgetComp.displayInEditor = !widgetComp.displayInEditor;
			}
			if (ImGui::IsItemHovered()&&ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				widgetComp.displayInEditor = true;
				ImGui::OpenPopup("##AddWidgetPopup");
			}
			DropWidgetTarget(nullptr, gameObj);

			if (!widgetComp.Widgets.empty()) {
				unsigned int texId = (widgetComp.displayInEditor) ? showChildrenTexture->GetId() : hiddenChildrenTexture->GetId();

				ImGui::SameLine();
				ImGui::SetNextItemAllowOverlap();
				ImGui::Image(texId, ImVec2(childrenTexSize, childrenTexSize));
			}

			if (widgetComp.displayInEditor) {
				DisplayWidgets(widgetComp, gameObj);
				WidgetOptionsPopup(widgetComp);
				CreateWidgetPopup(widgetComp);
			}

			ImGui::Unindent(20.0f);
			ImGui::PopStyleColor();
		}
	}
	void SceneDirectory::DisplayWidgets(WidgetCanvasComponent& widgetComp, std::shared_ptr<BtnSqd::GameObject>& gameObj) {
		ImGui::Indent(20.0f);
		unsigned int i = 0;
		for (auto& widget : widgetComp.Widgets) {
			if (widget->HasParent()) {
				continue;
			}
			DisplayWidget(widget.get(), widgetComp, gameObj, i);
		}
		ImGui::Unindent(20.0f);
	}
	void SceneDirectory::DisplayWidget(BtnWidget* widget, BtnSqd::WidgetCanvasComponent& widgetComp, std::shared_ptr<BtnSqd::GameObject>& gameObj, unsigned int i) {
		std::shared_ptr<BtnWidget>widgetSh = widget->shared_from_this();

		if (widget == widgetComp.selectedWidget.get()) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		std::string label = widget->GetName() + "##WidgetDisplayedID_" + std::to_string(gameObj->GetUUID()) + "_" + std::to_string(widget->GetId());
		if (ImGui::Selectable(label.c_str())) {
			widgetComp.selectedWidget = widgetSh;
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			widget->SetShowChildren(!widget->GetShowChildren());
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			widgetComp.selectedWidget = widgetSh;
			ImGui::OpenPopup("##WidgetOptionsPopup");
		}

		DragWidgetSource(widgetSh, gameObj);
		DropWidgetTarget(widgetSh, gameObj);

		if (widget->HasChildren()) {
			unsigned int texId = (widget->GetShowChildren()) ? showChildrenTexture->GetId() : hiddenChildrenTexture->GetId();

			ImGui::SameLine();
			ImGui::SetNextItemAllowOverlap();
			ImGui::Image(texId, ImVec2(childrenTexSize, childrenTexSize));


			ImGui::Indent(20.0f);
			if (widget->GetShowChildren()) {
				for (const auto& child : widget->GetChildren()) {
					auto childStrong = child.lock();

					if (childStrong) {
						DisplayWidget(childStrong.get(), widgetComp, gameObj, i);
					}
					else {
						widget->RemoveChild(child);
					}
				}
			}

			ImGui::Unindent(20.0f);
		}
		ImGui::PopStyleColor();
	}
	
	void SceneDirectory::DropAddChild(std::shared_ptr<BtnSqd::GameObject> gameObj) {
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_INFO")) {
				GameObjectPayload* data = static_cast<GameObjectPayload*>(payload->Data);

				GameObject* child = data->gameObject;

				if (child->GetParent()) {
					child->GetParent()->RemoveChild(*child);
				}

				std::shared_ptr<GameObject> parentObj;
				parentObj = gameObj;

				if (parentObj) {
					parentObj->AddChild(*child);
				}

			}
			ImGui::EndDragDropTarget();
		}
	}
	void SceneDirectory::DragGameObj(GameObject* gameObj) {
		if (ImGui::BeginDragDropSource()) {
			GameObjectPayload payload;
			payload.gameObject = gameObj;
			strcpy_s(payload.name, gameObj->GetComponent<TagComponenet>().tag.c_str());
			payload.assetType = AssetRawType::GameObjectRaw;

			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 100.0f);
			ImGui::Text("%s", payload.name);
			ImGui::PopTextWrapPos();

			ImGui::SetDragDropPayload("GAMEOBJECT_INFO", &payload, sizeof(GameObjectPayload));
			ImGui::EndDragDropSource();
		}
	}
	void SceneDirectory::DragWidgetSource(std::shared_ptr<BtnWidget>& widget, std::shared_ptr<BtnSqd::GameObject>& gameObj) {
		if (ImGui::BeginDragDropSource()) {
			WidgetPayload payload;
			payload.gameObject = gameObj.get();
			payload.widget = widget.get();
			strcpy_s(payload.name, widget->GetName().c_str());

			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 100.0f);
			ImGui::Text("%s", payload.name);
			ImGui::PopTextWrapPos();

			ImGui::SetDragDropPayload("WIDGET_INFO", &payload, sizeof(GameObjectPayload));
			ImGui::EndDragDropSource();
		}
	}
	void SceneDirectory::DropWidgetTarget(std::shared_ptr<BtnWidget> nParent, std::shared_ptr<BtnSqd::GameObject>& gameObj) {
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("WIDGET_INFO")) {
				WidgetPayload* data = static_cast<WidgetPayload*>(payload->Data);

				GameObject* baseGameObj = data->gameObject;
				if (baseGameObj->GetId() == gameObj->GetId()) {
					auto widget = data->widget->shared_from_this();
					if (widget->HasParent()) {
						widget->GetParent()->RemoveChild(widget);
					}

					if (nParent) {
						nParent->AddChild(widget);
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	void SceneDirectory::DestroySelected(GameObject& parent) {
		Application::GetApp()->PushEvent(new OnDesroyGameObjectEvent(parent));
	}
	void SceneDirectory::DisplayChildren(GameObject gameObj, GameObject& selectedObj) {
		ImGui::Indent(20.0f);
		for (auto& child : currentScene->GetgameObjects()[gameObj.GetUUID()]->GetChildren()) {
			if (selectedObj.IsValid() && child.GetUUID() == selectedObj.GetUUID()) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			}

			DisplayGameObject(currentScene->GetgameObjects()[child.GetUUID()], selectedObj);

			if (currentScene->GetgameObjects()[child.GetUUID()]->GetShowChildren()) {
				DisplayChildren(*currentScene->GetgameObjects()[child.GetUUID()], selectedObj);
				HandleDisplayWidgets(currentScene->GetgameObjects()[child.GetUUID()], selectedObj);
			}
			ImGui::PopStyleColor();
		}
		ImGui::Unindent(20.0f);
	}
	void SceneDirectory::SaveScene() {
		ImGui::Begin("Save Scene", &saveScene, ImGuiWindowFlags_NoResize);
		float childWidth = ImGui::GetContentRegionMax().x - 10.0f;
		float childHeight = 250.0f;
		ImGui::BeginChild("SaveDirectoryWind", ImVec2(childWidth, childHeight), true);

		std::filesystem::directory_iterator currentScenes(destination);
		for (auto& scn : currentScenes) {
			std::string tag = scn.path().filename().string() + "##SaveStateEntry";
			if (ImGui::Selectable(tag.c_str())) {
				std::string fileName = scn.path().filename().stem().string();
				strcpy_s(buffer, fileName.c_str());
			}
		}
		ImGui::EndChild();

		ImGui::InputText("##SaveSceneWindow", buffer, sizeof(buffer));
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			saveScene = false;
			SceneSerializer saveScene(currentScene.get());
			std::string nPath = destination + buffer + ".BtnScn";
			saveScene.Serialize(nPath, buffer);
			saveScene.Deserialize(nPath);
		}

		ImGui::End();
	}
	void SceneDirectory::LoadScene() {

		ImGui::Begin("Load Scene", &loadScene);
		ImVec2 loadSceneSize = ImGui::GetContentRegionMax();
		ImGui::BeginChild("##LoadSceneChildWind", ImVec2(loadSceneSize.x - 10.0f, loadSceneSize.y - 10.0f), true);
		for (auto& p : std::filesystem::directory_iterator(destination)) {
			std::string sceneName = p.path().filename().string();
			std::string sceneTag = sceneName + "##ChooseFileToLoad" + sceneName;

			if (ImGui::Selectable(sceneTag.c_str())) {
				Application::GetApp()->PushEvent(new OnEndRuntimeEvent());
				currentScene->UnLoadScene();
				loadScene = false;

				BtnScene* nScene = new BtnScene();
				SceneSerializer saveScene(nScene);
				saveScene.Deserialize(p.path().string());
				Application::GetApp()->PushEvent(new OnChangeSceneEvent(nScene));
				break;
			}
		}
		ImGui::EndChild();
		ImGui::End();
	}
	void SceneDirectory::SceneConfigMenue() {
		ImGui::Begin("Scene Config Menue", &sceneConfigMenue);

		ImGui::Text("Scene Name: ");

		char buffer[25];
		strcpy_s(buffer, currentScene->GetName().c_str());

		if (ImGui::InputText("##SceneNameConfig", buffer, sizeof(buffer))) {
			std::string nName = std::string(buffer);
			if (nName.size() == 0) {
				currentScene->SetName(std::string(" "));
			}
			else {
				currentScene->SetName(nName);
			}
		}
		ImGui::Selectable("Skybox", &changeSkybox);
		if (changeSkybox) {
			ChangeSkybox();
		}

		ImGui::End();
	}
	void SceneDirectory::ChangeSkybox() {
		ImVec2 childSize = ImVec2(200.0f, 100.0f);
		ImGui::BeginChild("##SkyboxChangeField", childSize);
		ImGui::Indent(20.0f);
		std::string skyboxDir = "./Assets/Textures/Skyboxes/";
		for (auto& p : std::filesystem::directory_iterator(skyboxDir)) {
			if (p.is_directory()) {
				std::string skyboxName = p.path().filename().string();
				if (ImGui::Selectable(skyboxName.c_str())) {
					currentScene->SetSkyboxTexture(p.path().string());
				}
			}
		}

		ImGui::Unindent(20.0f);
		ImGui::EndChild();
	}
}