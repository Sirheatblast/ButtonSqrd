#include "PropertiesMenue.h"

BtnSqd::PropertiesMenue::PropertiesMenue(std::shared_ptr<BtnScene>& scene, BtnPhysics& physics) :currentScene(scene), btnPhysics(physics) {
	widgetTypeDrawCallbacks[BtnWidgetType::Text] = [this](std::shared_ptr<BtnWidget> widget) {
		this->DrawBtnTextBoxData(widget);
		};
}

void BtnSqd::PropertiesMenue::OnUpdate(GameObject& selectedObj) {
	ImGui::Begin("Object properties");

	if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemActive()) {
		Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::MainGUI));
	}

	if (currentScene->GetgameObjects()[selectedObj.GetId()]->IsValid()) {
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2.0f) - 52.0f);
		if (ImGui::Button("Add Component")) {
			showAddCompWindow = !showAddCompWindow;
		}
		if (showAddCompWindow) {
			ImGui::OpenPopup("Add Component ##PopupWind");
			showAddCompWindow = false;
		}
		if (selectedObj.GetId() != selectedID) {
			UpdateParticleCurves(selectedObj);
			selectedID = selectedObj.GetId();
		}

		AddComponents(selectedObj);

		ImGui::Text(currentScene->GetgameObjects()[selectedObj.GetId()]->GetComponent<TagComponenet>().tag.c_str());
		ShowComponent<TagComponenet>(selectedObj);
		ShowComponent<TransformComponent>(selectedObj);
		ShowComponent<ModelComponent>(selectedObj);
		ShowComponent<LightComponent>(selectedObj);
		ShowComponent<CameraComponent>(selectedObj);
		ShowComponent<ScriptComponent>(selectedObj);
		ShowComponent<PhysicsComponet>(selectedObj);
		ShowComponent<ColliderComponent>(selectedObj);
		ShowComponent<ArmatureComponent>(selectedObj);
		ShowComponent<AnimatorComponent>(selectedObj);
		ShowComponent<BoneComponent>(selectedObj);
		ShowComponent<AudioListenerComponent>(selectedObj);
		ShowComponent<AudioSourceComponent>(selectedObj);
		ShowComponent<ParticleEmitterComponent>(selectedObj);
		ShowComponent<WidgetCanvasComponent>(selectedObj);
	}
	else {
		ImGui::Text("No GameObject Selected");
	}
	ImGui::End();
}

void BtnSqd::PropertiesMenue::AddComponents(BtnSqd::GameObject& selectedObj) {
	if (ImGui::BeginPopup("Add Component ##PopupWind", ImGuiPopupFlags_MouseButtonRight)) {
		if (ImGui::Selectable("Model Component")) {
			selectedObj.AddComponent<ModelComponent>();
		}
		if (ImGui::Selectable("Light Component")) {
			selectedObj.AddComponent<LightComponent>();
		}
		if (ImGui::Selectable("Script Component")) {
			selectedObj.AddComponent<ScriptComponent>();
		}
		if (ImGui::Selectable("Camera Component")) {
			selectedObj.AddComponent<CameraComponent>(selectedObj.GetComponent<TransformComponent>());
		}
		if (ImGui::Selectable("Physics Component")) {
			PhysicsComponet& physics = selectedObj.AddComponent<PhysicsComponet>();
			if (!selectedObj.CheckObjectForComponent<ColliderComponent>()) {
				selectedObj.AddComponent<ColliderComponent>();
			}
			CreateCollider(selectedObj, selectedObj.GetComponent<ColliderComponent>());
			btnPhysics.AddPhysicsObject(selectedObj);
		}
		if (ImGui::Selectable("Collider Component")) {
			ColliderComponent& collider = selectedObj.AddComponent<ColliderComponent>();
			CreateCollider(selectedObj, collider);
		}
		if (ImGui::Selectable("Armature Component")) {
			ArmatureComponent& armature = selectedObj.AddComponent<ArmatureComponent>();
			if (selectedObj.CheckObjectForComponent<ModelComponent>()) {
				std::string armaName = selectedObj.GetComponent<ModelComponent>().currentModel.GetDirectory();
				if (ResourceManager::GetLoadedArmatures().contains(armaName)) {
					armature.armature = ResourceManager::GetLoadedArmatures()[armaName];
					armature.armatureName = armaName;
				}
			}
		}
		if (ImGui::Selectable("Animator Component")) {
			AnimatorComponent& anim = selectedObj.AddComponent<AnimatorComponent>();
		}
		if (ImGui::Selectable("Bone Component")) {
			BoneComponent& bone = selectedObj.AddComponent<BoneComponent>();
			bone.targetId = selectedObj.GetId();
		}
		if (ImGui::Selectable("Audio Listener Component")) {
			AudioListenerComponent& anim = selectedObj.AddComponent<AudioListenerComponent>();
		}
		if (ImGui::Selectable("Audio Source Component")) {
			AudioSourceComponent& anim = selectedObj.AddComponent<AudioSourceComponent>();
		}
		if (ImGui::Selectable("Particle Emitter Component")) {
			ParticleEmitterComponent& partic = selectedObj.AddComponent<ParticleEmitterComponent>();
		}
		if (ImGui::Selectable("Widget Canvas Component")) {
			WidgetCanvasComponent wCanvas = selectedObj.AddComponent<WidgetCanvasComponent>();
		}
		ImGui::EndPopup();
	}
}

void BtnSqd::PropertiesMenue::SendEvent(Event* e) {
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<OnGetWidgetEvent>([this](OnGetWidgetEvent* e)->bool {
		return OnGetWidget(e);
	});
}

bool BtnSqd::PropertiesMenue::OnGetWidget(BtnSqd::OnGetWidgetEvent* e) {
	auto selectedObj = currentScene->GetgameObjects()[selectedID];
	if (selectedObj && selectedObj->CheckObjectForComponent<WidgetCanvasComponent>()) {
		auto& widgetComp = selectedObj->GetComponent<WidgetCanvasComponent>();
		const auto& widgets = widgetComp.Widgets;
		bool containsWidget = std::find(widgets.begin(), widgets.end(), e->GetWidget()) != widgets.end();
		if (containsWidget) {
			widgetComp.selectedWidget = e->GetWidget();
			Application::GetApp()->PushEvent(new OnSelectWidgetEvent(e->GetWidget()));
		}
		else {
			Application::GetApp()->PushEvent(new OnSelectWidgetEvent(std::shared_ptr<BtnWidget>()));
		}
	}
	return true;
}

void BtnSqd::PropertiesMenue::ShowVariableData(EditableData& data) {
	std::string label = "##ScriptVar_" + std::string(data.name);
	switch (data.type) {
	case DataType::Int: {
		int* asInt = static_cast<int*>(data.data);
		ImGui::DragInt(label.c_str(), asInt);
		return;
	}
	case DataType::Float: {
		float* asFloat = static_cast<float*>(data.data);
		ImGui::DragFloat(label.c_str(), asFloat);
		return;
	}
	case DataType::Double: {
		float* asDouble = static_cast<float*>(data.data);
		ImGui::DragFloat(label.c_str(), asDouble);
		return;
	}
	case DataType::Bool: {
		bool* asBool = static_cast<bool*>(data.data);
		ImGui::Checkbox(label.c_str(), asBool);
		return;
	}
	case DataType::String: {
		std::string* asString = static_cast<std::string*>(data.data);
		size_t size = asString->size();
		char buffer[100];
		strcpy_s(buffer, asString->c_str());
		ImGui::InputText(label.c_str(), buffer, ARRAYSIZE(buffer));
		return;
	}
	case DataType::GameObject: {
		GameObject* asGameObject = static_cast<GameObject*>(data.data);
		ImVec2 recSize{ 150.0f,21.0f };

		std::string tag;
		if (asGameObject->IsValid()) {
			tag = asGameObject->GetComponent<TagComponenet>().tag + " ##" + std::string(data.name);
		}
		else {
			tag = "Set GameObject ##" + std::string(data.name);
		}
		ImGui::Button(tag.c_str(), recSize);
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_INFO")) {

				GameObjectPayload* pData = static_cast<GameObjectPayload*>(payload->Data);
				*asGameObject = *pData->gameObject;
			}
			ImGui::EndDragDropTarget();
		}
		return;
	}
	case DataType::SuperGameObject: {
		ImVec2 recSize{ 150.0f,21.0f };
		SuperGameObject* asSuper = static_cast<SuperGameObject*>(data.data);
		std::string tag;
		if (asSuper->IsValid()) {
			tag = asSuper->GetTag().tag + " ##" + std::string(data.name);
		}
		else {
			tag = "Set SuperGameObject ##" + std::string(data.name);
		}
		ImGui::Button(tag.c_str(), recSize);
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_INFO")) {
				AssetPayloadType* pData = static_cast<AssetPayloadType*>(payload->Data);
				if (!ResourceManager::GetLoadedSuperGameObjects().contains(pData->path)) {
					std::shared_ptr<SuperGameObject> nSuper;
					GameObject temp;

					std::string currentName = pData->name;
					currentName.resize(currentName.size() - 6);
					for (auto& [gameObj, tag] : currentScene->GetRegister().GetAllOfID<TagComponenet>()) {
						if (tag.tag == currentName) {
							temp = *currentScene->GetgameObjects()[gameObj];
							break;
						}
					}

					nSuper.reset(new SuperGameObject(&temp, pData->path));
					ResourceManager::AddSuperGameObject("./Assets/SuperGameObjects/", nSuper);
				}
				*asSuper = *ResourceManager::GetLoadedSuperGameObjects()[pData->path];
			}
			ImGui::EndDragDropTarget();
		}
		return;
	}
	}
}

void BtnSqd::PropertiesMenue::ModelComp(BtnSqd::GameObject& selectedObj) {
	ModelComponent& modelComp = currentScene->GetgameObjects()[selectedObj.GetId()]->GetComponent<ModelComponent>();

	if (modelComp.currentModel.GetMeshes()->size() != showTexturePickerMain.size()) {
		showTexturePickerMain.clear();
		showTexturePickerNormal.clear();
		showColorPicker.clear();
		for (auto mesh = modelComp.currentModel.GetMeshes()->begin(); mesh < modelComp.currentModel.GetMeshes()->end(); mesh++) {
			showTexturePickerMain.push_back(false);
			showTexturePickerNormal.push_back(false);
			showColorPicker.push_back(false);
		}
	}

	ImGui::Text("Model Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveModel")) {
		selectedObj.RemoveComponent<ModelComponent>();
		return;
	}
	ImGui::Indent(20.0f);

	ImGui::Text("Model: ");
	ImGui::SameLine();

	std::string modelName = modelComp.currentModel.GetModelName() + " ##ModelSelect";
	if (modelComp.currentModel.GetModelName().size() == 0) {
		modelName = "Click to add Model ##ModelSelect";
	}

	ImGui::Selectable(modelName.c_str(), &showAddModel);
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_INFO")) {
			AssetPayloadType* pData = static_cast<AssetPayloadType*>(payload->Data);
			std::string fullPath = pData->path;
			SetModel(fullPath, modelComp);
		}
		ImGui::EndDragDropTarget();
	}

	if (showAddModel) {
		ImGui::BeginChild("##ModelSelectionWindow");
		ImGui::Indent(20.0f);
		for (auto& p : std::filesystem::directory_iterator("./Assets/Models/")) {
			std::string nPath = p.path().filename().string();
			if (p.is_directory()) {}

			else {
				if (ImGui::Selectable(nPath.c_str(), &showAddModel)) {
					std::string fullPath = p.path().generic_string();
					SetModel(fullPath, modelComp);
				}
			}
		}

		ImGui::Unindent(20.0f);
		ImGui::EndChild();
	}

	ImGui::Text("Meshes:");
	int i = 0;
	for (auto mesh = modelComp.currentModel.GetMeshes()->begin(); mesh < modelComp.currentModel.GetMeshes()->end(); mesh++) {
		ImGui::Indent(20.0f);
		std::string tag = "Mesh: ";
		tag += std::to_string(i);
		ImGui::Text(tag.c_str());
		ImGui::Text("Clear Color:");
		tag = "##ClearColorButton";
		tag += std::to_string(i);
		ImGui::SameLine();
		if (ImGui::ColorButton(tag.c_str(), ImVec4(mesh->GetMaterial()->clearColor.x, mesh->GetMaterial()->clearColor.y, mesh->GetMaterial()->clearColor.z, 1.0f))) {
			showColorPicker[i] = !showColorPicker[i];
		}
		if (showColorPicker[i]) {
			ShowColorPicker(mesh->GetMaterial()->clearColor, i);
		}
		ImGui::Text("Current Main Texture:");
		ImGui::SameLine();
		unsigned int mainTexId = (mesh->GetMaterial()->albedo != nullptr) ? mesh->GetMaterial()->albedo->GetId() : 0;
		tag = "##MainTextureButton";
		tag += std::to_string(i);
		if (ImGui::ImageButton(tag.c_str(), mainTexId, ImVec2(20.0f, 20.0f))) {
			showTexturePickerMain[i] = !showTexturePickerMain[i];
		}

		if (showTexturePickerMain[i]) {
			TexturePicker(mesh->GetMaterial()->albedo, showTexturePickerMain[i], mesh->GetMaterial()->clearColor, i);
		}

		ImGui::Text("Current Normal Texture:");
		ImGui::SameLine();
		unsigned int normalTexId = (mesh->GetMaterial()->normal != nullptr) ? mesh->GetMaterial()->normal->GetId() : 0;
		tag = "##NormalTextureButton";
		tag += std::to_string(i);
		if (ImGui::ImageButton(tag.c_str(), normalTexId, ImVec2(20.0f, 20.0f))) {
			showTexturePickerNormal[i] = !showTexturePickerNormal[i];
		}
		if (showTexturePickerNormal[i]) {
			TexturePicker(mesh->GetMaterial()->normal, showTexturePickerNormal[i], glm::vec3(), i);
		}
		ImGui::Text("Roughness: ");
		ImGui::SameLine();
		tag = " ##RoughnessInput";
		tag += std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &mesh->GetMaterial()->roughness, 0.01f, 0.01f, 1.0f);
		ImGui::Text("Reflectance: ");
		ImGui::SameLine();
		tag = " ##ReflectanceInput";
		tag += std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &mesh->GetMaterial()->reflectance, 0.01f, 0.0f, 1.0f);
		ImGui::Text("Metallic: ");
		ImGui::SameLine();
		tag = " ##MetallicInput";
		tag += std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &mesh->GetMaterial()->metallic, 0.01f, 0.0f, 1.0f);
		ImGui::Text("Transparency: ");
		ImGui::SameLine();
		tag = " ##Transparency";
		tag += std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &mesh->GetMaterial()->transparency, 0.01f, 0.0f, 1.0f);

		ImGui::Unindent(20.0f);
		i++;
	}
	ImGui::Unindent(20.0f);
}
void BtnSqd::PropertiesMenue::SetModel(std::string& fullPath, BtnSqd::ModelComponent& modelComp) {
	if (ResourceManager::GetLoadedModels().contains(fullPath)) {
		modelComp.currentModel = ResourceManager::GetLoadedModels()[fullPath];
		if (modelComp.currentModel.GetMeshes()->size() != showTexturePickerMain.size()) {
			showTexturePickerMain.clear();
			showTexturePickerNormal.clear();
			for (auto mesh = modelComp.currentModel.GetMeshes()->begin(); mesh < modelComp.currentModel.GetMeshes()->end(); mesh++) {
				showTexturePickerMain.push_back(false);
				showTexturePickerNormal.push_back(false);
			}
		}
	}
	else {
		ResourceManager::AddModel(fullPath, Model(fullPath, Shader::Create("./Assets/Shaders/PbrShader.vrt", "./Assets/Shaders/PbrShader.frag")));
		modelComp.currentModel = ResourceManager::GetLoadedModels()[fullPath];
	}

	showTexturePickerMain.clear();
	showTexturePickerNormal.clear();
	showColorPicker.clear();
	for (auto mesh = modelComp.currentModel.GetMeshes()->begin(); mesh < modelComp.currentModel.GetMeshes()->end(); mesh++) {
		showTexturePickerMain.push_back(false);
		showTexturePickerNormal.push_back(false);
		showColorPicker.push_back(false);
	}
}
void BtnSqd::PropertiesMenue::TransformComp(BtnSqd::GameObject& selectedObj) {
	TransformComponent& transform = currentScene->GetgameObjects()[selectedObj.GetId()]->GetComponent<TransformComponent>();
	ImGui::Text("Transform Component");
	ImGui::Indent(20.0f);
	ImGui::Text("Position: ");
	ImGui::SameLine();
	if (!selectedObj.IsChild()) {
		ImGui::DragFloat3(" ##PosInput", glm::value_ptr(transform.transform.GetTransformMat()[3]), 0.1f);
	}
	else {
		ImGui::DragFloat3(" ##PosInput", glm::value_ptr(transform.transform.localPosition), 0.1f);
	}

	ImGui::Text("Rotation: ");
	ImGui::SameLine();
	if (!selectedObj.IsChild()) {
		if (ImGui::DragFloat3(" ##RotationInput", glm::value_ptr(transform.transform.rotation), 0.1f)) {
			transform.transform.RotateTo(transform.transform.rotation);
		}
	}
	else {
		if (ImGui::DragFloat3(" ##RotationInput", glm::value_ptr(transform.transform.localRotation), 0.1f)) {
			transform.transform.RotateLocalTo(transform.transform.localRotation);
		}
	}

	ImGui::Text("Uniform Scale: ");
	ImGui::SameLine();
	ImGui::Checkbox("##UniformScale", &scaleUniform);
	if (!scaleUniform) {
		ImGui::Text("Scale: ");
		ImGui::SameLine();
		if (ImGui::DragFloat3(" ##ScaleInput", glm::value_ptr(transform.transform.scale), 0.1f)) {
			transform.transform.Scale(transform.transform.scale);
		}
	}
	else {
		ImGui::Text("Scale: ");
		ImGui::SameLine();
		float nScale = transform.transform.scale.x;
		if (ImGui::DragFloat(" ##ScaleInput", &nScale, 0.1f)) {
			transform.transform.scale = glm::vec3(nScale);
			transform.transform.Scale(transform.transform.scale);
		}
	}
	ImGui::Unindent(20.0f);
}
void BtnSqd::PropertiesMenue::TagComp(BtnSqd::GameObject& selectedObj) {
	TagComponenet& tag = currentScene->GetgameObjects()[selectedObj.GetId()]->GetComponent<TagComponenet>();
	ImGui::Text("Tag Component");
	ImGui::Indent(20.0f);
	ImGui::Text("Tag: ");
	ImGui::SameLine();

	char buffer[26];
	strcpy_s(buffer, tag.tag.c_str());

	ImGui::InputText("##TagInput", buffer, IM_ARRAYSIZE(buffer));
	tag.tag = std::string(buffer);
	if (tag.tag.empty()) {
		tag.tag.append(" ");
	}
	ImGui::Unindent(20.0f);
}
void BtnSqd::PropertiesMenue::LightComp(BtnSqd::GameObject& selectedObj) {
	LightComponent& light = currentScene->GetgameObjects()[selectedObj.GetId()]->GetComponent<LightComponent>();
	ImGui::Text("Light Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveLight")) {
		selectedObj.RemoveComponent<LightComponent>();
		return;
	}
	ImGui::Indent(20.0f);
	ImGui::Text("Light Type: ");
	ImGui::SameLine();
	static const char* items[] = { "Directional", "Point", "Spot" };
	int choice = light.lightType;
	if (ImGui::Combo("##LightTypeCombo", &choice, items, IM_ARRAYSIZE(items))) {
		light.lightType = LightType(choice);
	}

	ImGui::Text("Strength: ");
	ImGui::SameLine();
	ImGui::DragFloat("##DragLStrength", &light.strength, 1.0f, 0.0f, 1000.0f);

	ImGui::Text("Color: ");
	ImGui::SameLine();
	if (ImGui::ColorButton("##LightColorButton", ImVec4(light.lightColor.x, light.lightColor.y, light.lightColor.z, 1.0f))) {
		lightColorPicker = !lightColorPicker;
	}
	if (lightColorPicker) {
		ShowColorPicker(light.lightColor, selectedObj.GetId());
	}

	if (light.lightType == LightType::SpotLight) {
		ImGui::Text("Cutoff: ");
		ImGui::SameLine();
		ImGui::DragFloat("##DragLCutoff", &light.cutOff, 0.5f, 0.0f, light.outerCutOff);
		ImGui::Text("Outer Cutoff: ");
		ImGui::SameLine();
		ImGui::DragFloat("##DragLOCutoff", &light.outerCutOff, 0.5f, light.cutOff, 360.0f);
	}

	ImGui::Unindent(20.0f);
}
void BtnSqd::PropertiesMenue::CameraComp(GameObject& selectedObj) {
	CameraComponent& camera = currentScene->GetgameObjects()[selectedObj.GetId()]->GetComponent<CameraComponent>();
	ImGui::Text("Camera Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveCamera")) {
		selectedObj.RemoveComponent<CameraComponent>();
		return;
	}
	ImGui::Indent(20.0f);
	ImGui::Text("Is main Camera: ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##IsMainCamera", &camera.isMainCamera)) {
		Application::GetApp()->PushEvent(new OnSetActiveCameraEvent(selectedObj));
	}

	ImGui::Text("FOV: ");
	ImGui::SameLine();
	if (ImGui::DragFloat("##FOVSlider", &camera.fov, 0.5f, 60.0f, 120.0f)) {
		camera.projectionMatrix = glm::perspective(glm::radians(camera.fov), camera.aspectRatio, camera.nearPlain, camera.farPlain);
	}

	ImGui::Text("Near Plane: ");
	ImGui::SameLine();
	if (ImGui::DragFloat("##NearPlaneSlider", &camera.nearPlain)) {
		camera.projectionMatrix = glm::perspective(glm::radians(camera.fov), camera.aspectRatio, camera.nearPlain, camera.farPlain);
	}

	ImGui::Text("Far Plane: ");
	ImGui::SameLine();
	if (ImGui::DragFloat("##FarPlaneSlider", &camera.farPlain)) {
		camera.projectionMatrix = glm::perspective(glm::radians(camera.fov), camera.aspectRatio, camera.nearPlain, camera.farPlain);
	}
	ImGui::Unindent(20.0f);
}
void BtnSqd::PropertiesMenue::ScriptComp(GameObject& selectedObj) {
	ScriptComponent& scripts = currentScene->GetgameObjects()[selectedObj.GetId()]->GetComponent<ScriptComponent>();
	ImGui::Text("Script Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveScript")) {
		selectedObj.RemoveComponent<ScriptComponent>();
		return;
	}
	ImGui::Indent(20.0f);
	if (ImGui::Button("Add Script")) {
		showAddScript = !showAddScript;
	}
	if (showAddScript) {
		showAddScript = false;
		ImGui::OpenPopup("Add Script ##PopupAddScriptWind", ImGuiPopupFlags_MouseButtonRight);
	}

	AddScriptPopup(selectedObj, scripts);

	for (auto& scriptEntry : scripts.scripts) {
		std::string buttonName = "Remove Script ##" + scriptEntry.name;
		if (ImGui::Button(buttonName.c_str())) {
			scripts.RemoveScript(scriptEntry.name);
			continue;
		}
		ImGui::Text(scriptEntry.name.c_str());
		ImGui::SameLine();
		std::string label = "##IsScriptEnabled_" + scriptEntry.name;
		ImGui::Checkbox(label.c_str(), &scriptEntry.isEnabled);

		ImGui::Indent(20.0f);
		//Display all of the script variables that can be modifed in the editor
		uint32_t count = 0;
		EditableData* editable = scriptEntry.script->GetEditables(count);
		for (unsigned int i = 0; i < count; i++) {
			auto element = editable[i];
			ImGui::Text(element.name);
			ImGui::SameLine();
			ShowVariableData(element);
		}

		ImGui::Unindent(20.0f);

	}
	ImGui::Unindent(20.0f);
}
void BtnSqd::PropertiesMenue::PhysicsComp(GameObject& selectedObj) {
	ImGui::Text("Physics Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemovePhysics")) {
		selectedObj.RemoveComponent<PhysicsComponet>();
		return;
	}
	PhysicsComponet& physicsComp = selectedObj.GetComponent<PhysicsComponet>();

	ImGui::Indent(20.0f);
	ImGui::Text("Rotation Lock: ");
	ImGui::SameLine();


	ImGui::BeginGroup();
	ImGui::Text(" X "); ImGui::SameLine();
	ImGui::Text(" Y"); ImGui::SameLine();
	ImGui::Text(" Z");

	if (ImGui::Checkbox("##LockX", &physicsComp.lockX)) {
		btnPhysics.AddPhysicsObject(selectedObj);
	}
	ImGui::SameLine();;
	if (ImGui::Checkbox("##LockY", &physicsComp.lockY)) {
		btnPhysics.AddPhysicsObject(selectedObj);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("##LockZ", &physicsComp.lockZ)) {
		btnPhysics.AddPhysicsObject(selectedObj);
	}
	ImGui::EndGroup();

	ImGui::Text("Is Dynamic: ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##IsDynamic", &physicsComp.isDynamic)) {
		btnPhysics.AddPhysicsObject(selectedObj);
	}
	ImGui::Text("Is Kinematic: ");
	ImGui::SameLine();
	if (ImGui::Checkbox(" ##IsKinematic", &physicsComp.isKinematic)) {
		btnPhysics.AddPhysicsObject(selectedObj);
	}

	ImGui::Text("Mass: ");
	ImGui::SameLine();
	ImGui::DragFloat(" ##Mass", &physicsComp.mass, 1.0f, 0.0f, FLT_MAX);

	ImGui::Unindent(20.0f);
}
void BtnSqd::PropertiesMenue::ColliderComp(GameObject& selectedObj) {
	ImGui::Text("Colider Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveColider")) {
		selectedObj.RemoveComponent<ColliderComponent>();
		return;
	}
	ImGui::Indent(20.0f);

	ColliderComponent& collider = selectedObj.GetComponent<ColliderComponent>();

	ImGui::Text("Static Friction: ");
	ImGui::SameLine();
	if (ImGui::DragFloat(" ##StaticFriction", &collider.staticFriction, 0.1f, 0.0f, FLT_MAX)) {
		CreateCollider(selectedObj, collider);
	}

	ImGui::Text("Dynamic Friction: ");
	ImGui::SameLine();
	if (ImGui::DragFloat(" ##DynamicFriction", &collider.dynamicFriction, 0.1f, 0.0f, FLT_MAX)) {
		CreateCollider(selectedObj, collider);
	}

	ImGui::Text("resitution: ");
	ImGui::SameLine();
	if (ImGui::DragFloat(" ##resitution", &collider.resitution, 0.1f, 0.0f, 1.0f)) {
		CreateCollider(selectedObj, collider);
	}

	ImGui::Text("Is Selectable ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ColliderSelectableTag", &collider.isSelectalbe)) {
		CreateCollider(selectedObj, collider);
	}

	ImGui::Text("Is Trigger ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ColliderTriggerTag", &collider.isTrigger)) {
		CreateCollider(selectedObj, collider);
	}

	static const char* items[] = { "Box","Sphere", "Capsule","Plane","Custom" };
	int choice = collider.colliderType;

	ImGui::Text("Colider Type: ");
	ImGui::SameLine();
	if (ImGui::Combo("##ColiderType", &choice, items, IM_ARRAYSIZE(items))) {
		collider.colliderType = ColliderType(choice);
		CreateCollider(selectedObj, collider);
	}
	glm::vec3 transformScale = selectedObj.GetComponent<TransformComponent>().transform.scale;
	switch (collider.colliderType) {
	case ColliderType::BoxCollider:
		ImGui::Text("Use Transform Scale: ");
		ImGui::SameLine();
		ImGui::Checkbox(" ##UseSameScaleAsTransform", &collider.sameAsTransform);
		if (!collider.sameAsTransform) {
			ImGui::Text("Scale: ");
			ImGui::SameLine();
			if (ImGui::DragFloat3(" ##ColliderScale", glm::value_ptr(collider.scale), 0.1f, 0.1f, FLT_MAX)) {
				CreateCollider(selectedObj, collider);
			}
		}
		else {
			if (collider.scale != transformScale) {
				collider.scale = transformScale;
				CreateCollider(selectedObj, collider);
			}
		}
		break;
	case ColliderType::PlaneCollider:
		ImGui::Text("Use Transform Scale: ");
		ImGui::SameLine();
		ImGui::Checkbox(" ##UseSameScaleAsTransform", &collider.sameAsTransform);
		if (!collider.sameAsTransform) {
			ImGui::Text("Scale: ");
			ImGui::SameLine();
			if (ImGui::DragFloat3(" ##ColliderScale", glm::value_ptr(collider.scale), 0.1f, 0.1f, FLT_MAX)) {
				collider.scale.y = 0.1f;
				CreateCollider(selectedObj, collider);
			}
		}
		else {
			transformScale.y = .1f;
			if (collider.scale != transformScale) {
				collider.scale = transformScale;
				CreateCollider(selectedObj, collider);
			}
		}
		break;
	case ColliderType::CircleCollider:
		ImGui::Text("Use Transform Scale: ");
		ImGui::SameLine();
		ImGui::Checkbox(" ##UseSameScaleAsTransform", &collider.sameAsTransform);
		if (!collider.sameAsTransform) {
			ImGui::Text("Radius: ");
			ImGui::SameLine();
			if (ImGui::DragFloat(" ##RadiusCollider", &collider.radius, 0.1f, 0.1f, FLT_MAX)) {
				CreateCollider(selectedObj, collider);
			}
		}
		else {
			if (collider.radius != transformScale.x && transformScale.x >= 0.1f) {
				collider.radius = transformScale.x;
				CreateCollider(selectedObj, collider);
			}
		}
		break;
	case ColliderType::CapsuleCollider:
		ImGui::Text("Use Transform Scale: ");
		ImGui::SameLine();
		ImGui::Checkbox(" ##UseSameScaleAsTransform", &collider.sameAsTransform);
		if (!collider.sameAsTransform) {
			ImGui::Text("Radius: ");
			ImGui::SameLine();
			if (ImGui::DragFloat(" ##RadiusCollider", &collider.radius, 0.1f, 0.1f, FLT_MAX)) {
				CreateCollider(selectedObj, collider);
			}
			ImGui::Text("Half Height: ");
			ImGui::SameLine();
			if (ImGui::DragFloat(" ##HalfHeightollider", &collider.halfHeight, 0.1f, 0.1f, FLT_MAX)) {
				CreateCollider(selectedObj, collider);
			}
		}
		else {
			if (collider.radius != transformScale.x && transformScale.x >= 0.1f && transformScale.y >= 0.1f) {
				collider.radius = transformScale.x;
				collider.halfHeight = transformScale.y;
				CreateCollider(selectedObj, collider);
			}
		}
		break;
	}
	ImGui::Unindent(20.0f);
}

void BtnSqd::PropertiesMenue::ArmatureComp(GameObject& selectedObj) {
	ImGui::Text("Armature Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveArmature")) {
		selectedObj.RemoveComponent<ArmatureComponent>();
		return;
	}

	ImGui::Indent(20.0f);
	ArmatureComponent& armatureComponent = selectedObj.GetComponent<ArmatureComponent>();
	std::string currentArmatureName = "Select Armature";
	if (!armatureComponent.armatureName.empty()) {
		currentArmatureName = armatureComponent.armatureName;
	}

	if (ImGui::BeginCombo("##ArmatureCombo", currentArmatureName.c_str())) {
		for (auto [armName, armature] : ResourceManager::GetLoadedArmatures()) {
			if (ImGui::Selectable((armName + "##ArmatureComboTag").c_str())) {

			}
		}

		ImGui::EndCombo();
	}
	ImGui::Unindent(20.0f);
}

void BtnSqd::PropertiesMenue::AnimatorComp(GameObject& selectedObj) {
	ImGui::Text("Animator Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveAnimator")) {
		selectedObj.RemoveComponent<AnimatorComponent>();
		return;
	}
	ImGui::Indent(20.0f);

	AnimatorComponent& anim = selectedObj.GetComponent<AnimatorComponent>();
	std::string currentAnimationName = "Select Animation";
	if (!anim.currentAnimation.animation.GetName().empty()) {
		currentAnimationName = anim.currentAnimation.animation.GetName();
	}

	if (ImGui::BeginCombo("##currentAnimCombo", currentAnimationName.c_str())) {
		for (auto aniPack : anim.animations) {
			if (aniPack && ImGui::Selectable((aniPack.value().animation.GetName() + "##AnimationComboTag").c_str())) {
				anim.currentAnimation.isPlaying = false;
				anim.currentAnimation.animation = aniPack.value().animation;
				anim.currentAnimation.isLooping = aniPack.value().isLooping;
				anim.currentAnimation.playbackSpeed = aniPack.value().playbackSpeed;
				anim.currentTime = 0;
			}
		}

		ImGui::EndCombo();
	}
	AnimationPack& current = anim.currentAnimation;
	ImGui::Text("Animation Settings:");
	ImGui::BeginChild("##Animationinfo", ImVec2(300.0f, 100.0f), true);

	ImGui::Text("Is Looping: ");
	ImGui::SameLine();
	ImGui::Checkbox("##animaitonLoopCheck", &current.isLooping);

	ImGui::Text("Playback Speed: ");
	ImGui::SameLine();
	ImGui::DragFloat("##animaitonPlaySpeedDragFloat", &current.playbackSpeed, 0.01f, 0.0f, FLT_MAX);
	ImGui::EndChild();

	std::string aniState = (current.isPlaying) ? "Stop" : "Play";

	if (ImGui::Button(aniState.c_str())) {
		current.isPlaying = !current.isPlaying;
	}

	ImGui::Unindent(20.0f);
}

void BtnSqd::PropertiesMenue::BoneComp(GameObject& selectedObj) {
	ImGui::Text("Bone Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveBoneComp")) {
		selectedObj.RemoveComponent<BoneComponent>();
		return;
	}
	ImGui::Indent(20.0f);

	BoneComponent& bone = selectedObj.GetComponent<BoneComponent>();
	std::string currentBone = (!bone.selectedBone || bone.selectedBone->name == "") ? "Select Bone" : bone.selectedBone->name;

	ImGui::Text("Copy Bone Position: ");
	ImGui::SameLine();
	ImGui::Checkbox("##CopyBonePosCheckBox", &bone.copyPos);

	ImGui::Text("Copy Bone Rotation: ");
	ImGui::SameLine();
	ImGui::Checkbox("##CopyBoneRotationCheckBox", &bone.copyRot);

	ImGui::Text("Use Parent Armature: ");
	ImGui::SameLine();
	ImGui::Checkbox("##UseParentArmatureCheckBox", &bone.useParentArmature);

	if (bone.useParentArmature) {
		if (selectedObj.GetParent()) {
			bone.armatureId = selectedObj.GetParent()->GetId();
		}
	}
	else {
		ImVec2 recSize{ 150.0f,21.0f };

		std::string label = (bone.armatureName.empty()) ? "Drag GameObject here" : bone.armatureName;

		ImGui::Button(label.c_str(), recSize);
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_INFO")) {
				GameObjectPayload* pData = static_cast<GameObjectPayload*>(payload->Data);
				bone.armatureId = pData->gameObject->GetId();
				bone.armatureName = pData->name;
				bone.selectedBone = nullptr;
			}
			ImGui::EndDragDropTarget();
		}
	}

	if (ImGui::BeginCombo("##SetBoneCombo", currentBone.c_str())) {
		if (currentScene->GetgameObjects()[bone.armatureId]->CheckObjectForComponent<ArmatureComponent>()) {
			ArmatureComponent& armature = currentScene->GetgameObjects()[bone.armatureId]->GetComponent<ArmatureComponent>();
			std::string label;
			for (auto& bne : armature.armature.GetBones()) {
				label = bne.name + "##ArmBone";
				if (ImGui::Selectable(label.c_str())) {
					bone.selectedBone = &bne;
					bone.boneName = bne.name;
					break;
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Unindent(20.0f);
}

void BtnSqd::PropertiesMenue::AudioListenerComp(GameObject& selectedObj) {
	ImGui::Text("Audio Listner Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveAudioListenerComp")) {
		selectedObj.RemoveComponent<AudioListenerComponent>();
		return;
	}
}

void BtnSqd::PropertiesMenue::AudioSourceComp(GameObject& selectedObj) {
	ImGui::Text("Audio Source Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveAudioSourceComp")) {
		selectedObj.RemoveComponent<AudioSourceComponent>();
		return;
	}
	AudioSourceComponent& audioSrc = selectedObj.GetComponent<AudioSourceComponent>();

	ImGui::Indent(20.0f);

	if (ImGui::Button("Add Sound##AddSoundToSource")) {
		addSound = !addSound;
	}
	ImGui::BeginChild("##AudioSourceInfoDisplay", ImVec2(300.0f, 200.0f), true);
	ImGui::Text("Sounds:");
	for (const auto [soundName, SoundInfo] : audioSrc.audioBuffers) {
		std::string label = soundName + "##ASourceInfoDisplay";
		if (ImGui::Selectable(label.c_str(), false, false, ImVec2(225.0f, 0.0f))) {
			audioSrc.source.SetAudioBuffer(SoundInfo.id);
			audioSrc.currentSoundName = soundName;
		}
		ImGui::SameLine();
		if (ImGui::Button(("Remove##Remove" + label).c_str())) {
			audioSrc.audioBuffers.erase(soundName);
			if (audioSrc.currentSoundName == soundName) {
				audioSrc.currentSoundName = "";
			}
			break;
		}
	}
	ImGui::EndChild();
	ImGui::BeginChild("##AudioSourceAudioPlayer", ImVec2(300.0f, 0.0f), true);
	ImGui::Text("Sound Player:");
	ImGui::Indent(20.0f);

	ImGui::Text("Active Sound: ");
	ImGui::SameLine();
	ImGui::Text(audioSrc.currentSoundName.c_str());

	std::string buttonlabel = (!audioSrc.source.IsPlaying()) ? "Play" : "Stop";
	if (ImGui::Button(buttonlabel.c_str())) {
		if (audioSrc.source.IsPlaying()) {
			audioSrc.source.Stop();
		}
		else {
			audioSrc.source.Play();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause")) {
		if (audioSrc.source.IsPlaying()) {
			audioSrc.source.Pause();
		}
	}
	if (audioSrc.audioBuffers.contains(audioSrc.currentSoundName)) {
		ImGui::Text("Is Looping: ");
		ImGui::SameLine();
		if (ImGui::Checkbox("##IsSoundLooping", &audioSrc.audioBuffers[audioSrc.currentSoundName].isLooping)) {
			audioSrc.source.SetLooping(audioSrc.audioBuffers[audioSrc.currentSoundName].isLooping);
		}

		ImGui::Text("Gain: ");
		ImGui::SameLine();
		if (ImGui::DragFloat("##SetAudioSrcGain", &audioSrc.audioBuffers[audioSrc.currentSoundName].gain, 0.05f, 0.0f, 10.0f)) {
			audioSrc.source.SetGain(audioSrc.audioBuffers[audioSrc.currentSoundName].gain);
		}

		ImGui::Text("Pitch: ");
		ImGui::SameLine();
		if (ImGui::DragFloat("##SetAudioSrcPitch", &audioSrc.audioBuffers[audioSrc.currentSoundName].pitch, 0.1f, 0.0f, 100.0f)) {
			audioSrc.source.SetPitch(audioSrc.audioBuffers[audioSrc.currentSoundName].pitch);
		}
	}


	ImGui::Unindent(20.0f);
	ImGui::EndChild();

	if (addSound) {
		ImGui::OpenPopup("##AddSoundPopup");
		addSound = false;
	}

	if (ImGui::BeginPopup("##AddSoundPopup")) {
		for (auto& [soundName, soundBuffer] : ResourceManager::GetLoadedSounds()) {
			std::string label = soundName + "##AddSoundPopupSound";
			if (ImGui::Selectable(label.c_str())) {
				audioSrc.audioBuffers.insert({ soundName,{soundBuffer->GetId()} });
			}
		}

		ImGui::EndPopup();
	}

	ImGui::Unindent(20.0f);
}

void BtnSqd::PropertiesMenue::ParticleEmitterComp(GameObject& selectedObj) {
	ImGui::Text("Particle Emitter Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveParticleEmitterComp")) {
		selectedObj.RemoveComponent<ParticleEmitterComponent>();
		return;
	}
	ParticleEmitterComponent& partic = selectedObj.GetComponent<ParticleEmitterComponent>();

	ImGui::Indent(20.0f);

	ImGui::BeginChild("ParticleEmmiterSubWindow", ImVec2(400.0f, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoScrollbar);

	if (ImGui::Button("New Particle##AddParticleButton")) {
		partic.particles.push_back(BtnParticle());
		partic.particles[partic.particles.size() - 1].genCurveTextures = true;

		UpdateParticleCurves(selectedObj);
	}

	DrawParticleGUI(partic, selectedObj);

	ImGui::EndChild();

	std::string label = (partic.isEmiting == false) ? "Emit" : "stop";
	if (ImGui::Button(label.c_str())) {
		partic.isEmiting = !partic.isEmiting;
	}
	if (showCurve) {
		ImGui::BeginChild("##EmitterCurveEditor", ImVec2(0.0f, 0.0f), true);
		if (ImGui::Button("Close Curve Editor##CollapseCurveEditor")) {
			showCurve = false;
		}
		DrawCurve(partic);
		ImGui::EndChild();
	}


	ImGui::BeginChild("#EmitterCompSettings", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY);
	ImGui::Text("Emitter Settings: ");

	std::string currentModeName;
	if (partic.eMode == ParticleEmitionMode::Linear) {
		currentModeName = "Linear";
	}
	else if (partic.eMode == ParticleEmitionMode::Random) {
		currentModeName = "Random";
	}
	else {
		currentModeName = "Selected";
	}

	ImGui::Text("Emitter Mode:");
	ImGui::SameLine();
	if (ImGui::BeginCombo(" ##EmitterModeCombo", currentModeName.c_str())) {
		if (ImGui::Selectable("Linear##EmitterMode")) {
			partic.eMode = ParticleEmitionMode::Linear;
		}
		if (ImGui::Selectable("Random##EmitterMode")) {
			partic.eMode = ParticleEmitionMode::Random;
		}
		if (ImGui::Selectable("Selected##EmitterMode")) {
			partic.eMode = ParticleEmitionMode::Selected;
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Max particles: ");
	ImGui::SameLine();
	ImGui::DragScalar("##EmitterCompMaxParticlesDragInt", ImGuiDataType_U32, &partic.maxParticles);

	ImGui::Text("Repeat emission: ");
	ImGui::SameLine();
	ImGui::Checkbox("##EmitterCompDoesRepeat", &partic.isRepeating);

	ImGui::Text("Repeat Timer: ");
	ImGui::SameLine();
	ImGui::DragFloat("##EmitterCompDoesRepeatTimer", &partic.repeatMaxTime, 0.5f, 0.0f, FLT_MAX);

	if (partic.eMode == ParticleEmitionMode::Selected) {
		ImGui::Text("Selected Particle ID: ");
		ImGui::SameLine();
		if (ImGui::DragScalar("##EmitterCompSelectedParticleDragInt", ImGuiDataType_U32, &partic.selectedParticleID)) {
			if (partic.selectedParticleID >= partic.particles.size())
				partic.selectedParticleID = partic.particles.size() - 1;
		}
	}

	ImGui::EndChild();

	ImGui::Unindent(20.0f);
}

void BtnSqd::PropertiesMenue::WidgetCanvasComp(GameObject& selectedObj) {
	ImGui::Text("Widget Canvas Component");
	ImGui::SameLine();
	if (ImGui::Button("Remove ##RemoveWidgetCanvasComp")) {
		selectedObj.RemoveComponent<WidgetCanvasComponent>();
		return;
	}

	auto& wCanvas = selectedObj.GetComponent<WidgetCanvasComponent>();

	ImGui::Indent(20.0f);
	ImGui::Text("Widgets:");
	ImGui::SameLine();
	if (!showGui) {
		if (ImGui::Button("Edit Widgets##wCanvasEditWidgetButton")) {
			Application::GetApp()->PushEvent(new OnEnableGuiEvent("EditorGui"));
			Application::GetApp()->PushEvent(new OnEditWidgetEnableEvent("EditorGui"));
			showGui = true;
		}
	}
	else {
		if (ImGui::Button("Finish Editing Widgets##wCanvasEditWidgetButton")) {
			Application::GetApp()->PushEvent(new OnDisableGuiEvent("EditorGui"));
			Application::GetApp()->PushEvent(new OnEditWidgetDisableEvent("EditorGui"));
			showGui = false;
		}
	}
	
	ImGui::BeginChild("##WidgetCanvasShowWidgetsWindow", ImVec2(0.0f, 0.0f), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);
	float addWidgetButtonWidth = 100.0f;
	float buttonOffset = (ImGui::GetContentRegionAvail().x - addWidgetButtonWidth) * 0.5f;
	if (buttonOffset > 0) {
		ImGui::SetCursorPosX(buttonOffset);
	}
	if (ImGui::Button("Add Widget##AddWidgetWCanvasButton", ImVec2(addWidgetButtonWidth, 0.0f))) {
		ImGui::OpenPopup("##AddWidgetPopup");
	}

	CreateWidgetPopup(wCanvas);

	unsigned int idx = 0;
	for (const auto& widget : wCanvas.Widgets) {
		if (ImGui::Selectable((widget->GetName() + "##Widget" + std::to_string(idx)).c_str())) {
			wCanvas.selectedWidget = widget;
		}
		idx++;
	}

	ImGui::EndChild();

	ImGui::Text("Selected Widget:");
	ImGui::BeginChild("##WidgetCanvasSelectedWidgetWindow", ImVec2(0.0f, 0.0f), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);

	if (wCanvas.selectedWidget) {
		char buf[100];
		strcpy_s(buf, sizeof(buf), wCanvas.selectedWidget->GetName().c_str());
		if (ImGui::InputText("##SelectedWidgetEditName", buf, sizeof(buf))) {
			wCanvas.selectedWidget->SetName(buf);
		}
		widgetTypeDrawCallbacks[wCanvas.selectedWidget->GetType()](wCanvas.selectedWidget);

		bool useDim = wCanvas.selectedWidget->GetUseScreenDim();
		ImGui::Text("Use Screen Dimensions ");
		ImGui::SameLine();
		if (ImGui::Checkbox("##WidgetCanvasSelectedWidgetUseScreenDim",&useDim)) {
			wCanvas.selectedWidget->SetUseScreenDim(useDim);
		}

		ImGui::Text("Position: ");
		ImGui::SameLine();
		if (wCanvas.selectedWidget->GetUseScreenDim()) {
			float max = 100.0f;
			glm::vec2 windSize;
			ImGui::Begin("Game Viewport");
			windSize = { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y };
			ImGui::End();
			glm::vec2 wPos = wCanvas.selectedWidget->GetPercentPos();
			ImGui::DragFloat2("##SelectedWidgetDragPosition", glm::value_ptr(wPos),1.0f,0.0f,max);
			glm::vec2 wPercent = wPos / max;
			wCanvas.selectedWidget->SetPosPercent(wPos);
		}
		else {
			glm::vec2 wPos = wCanvas.selectedWidget->GetPos();
			if (ImGui::DragFloat2("##SelectedWidgetDragPosition", glm::value_ptr(wPos))) {
				wCanvas.selectedWidget->SetPos(wPos);
			}
		}
		

		ImGui::Text("Dimensions: ");
		ImGui::SameLine();
		glm::vec2 dimension = wCanvas.selectedWidget->GetDimensions();
		if(ImGui::DragFloat2("##SelectedWidgetDragDimensions", glm::value_ptr(dimension))){
			wCanvas.selectedWidget->SetDimensions(dimension);
		}
	}

	ImGui::EndChild();

	ImGui::Text("Canvas Settings:");
	ImGui::BeginChild("##WidgetCanvasSettingsWindow", ImVec2(0.0f, 0.0f), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);

	ImGui::Text("Use Full screen: ");
	ImGui::SameLine();
	ImGui::Checkbox("##WidgetCanvasUseWholeScreenCheck", &wCanvas.useWholeScreen);

	if (!wCanvas.useWholeScreen) {
		ImGui::Text("Canvas Width: ");
		ImGui::SameLine();
		ImGui::DragInt("##DragWidgetCanvasWidth", &wCanvas.dimensions.x, 1, 0, INT_MAX, "%u");

		ImGui::Text("Canvas Height: ");
		ImGui::SameLine();
		ImGui::DragInt("##DragWidgetCanvasHeight", &wCanvas.dimensions.y, 1, 0, INT_MAX, "%u");
	}

	ImGui::EndChild();
	ImGui::Unindent(20.0f);
}

void BtnSqd::PropertiesMenue::CreateWidgetPopup(WidgetCanvasComponent& wCanvas) {
	if (ImGui::BeginPopup("##AddWidgetPopup")) {
		if (ImGui::Selectable("Text Box")) {
			wCanvas.Widgets.push_back(std::make_shared<BtnTextBox>());
		}
		if (ImGui::Selectable("Button")) {

		}
		if (ImGui::Selectable("Image")) {

		}
		if (ImGui::Selectable("Slider")) {

		}
		ImGui::EndPopup();
	}
}

void BtnSqd::PropertiesMenue::DrawParticleGUI(BtnSqd::ParticleEmitterComponent& partic, BtnSqd::GameObject& selectedObj) {
	unsigned int idx = 0;
	std::hash<std::string> labelHasher;

	for (auto& p : partic.particles) {
		ImGui::PushID(("ParticleID" + std::to_string(idx)).c_str());

		std::string pName = "##DisplayParticle_";
		pName += idx;
		ImGui::BeginChild(pName.c_str(), ImVec2(0.0f, 0.0f), ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		std::string disPName = "Particle: " + std::to_string(idx);
		ImGui::Text(disPName.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Remove##RemoveParticleFromEmitter")) {
			partic.particles.erase(partic.particles.begin() + idx);
			UpdateParticleCurves(selectedObj);
			ImGui::EndChild();
			ImGui::PopID();
			return;
		}

		std::string fieldLabel = pName + "ClearColor" + std::to_string(idx);
		ImGui::Text("Clear Color");
		ImGui::SameLine();
		if (ImGui::ColorButton(("##ClearColorButton" + std::to_string(idx)).c_str(), ImVec4(p.clearColor.r, p.clearColor.g, p.clearColor.b, p.clearColor.a))) {
			p.changeClearColor = !p.changeClearColor;
		}
		if (p.changeClearColor) {
			ShowColorPicker(p.clearColor, fieldLabel.c_str());
		}

		ImGui::Text("Use Transition Color: ");
		ImGui::SameLine();
		ImGui::Checkbox(("##EnableTransitionParticleColor" + std::to_string(idx)).c_str(), &p.enableColorT);

		if (p.enableColorT) {
			ImGui::Indent(20.0f);
			fieldLabel = pName + "TransitionColor" + std::to_string(idx);
			ImGui::Text("Transition Color:");
			ImGui::SameLine();
			if (ImGui::ColorButton(("##TransitionColorButton#" + std::to_string(idx)).c_str(), ImVec4(p.transitionColor.r, p.transitionColor.g, p.transitionColor.b, p.transitionColor.a))) {
				p.changeTColor = !p.changeTColor;
			}
			if (p.changeTColor) {
				ShowColorPicker(p.transitionColor, fieldLabel.c_str());
			}
			RenderCurveConfig(idx, selectedObj, true, p.useColorCurve, p.colorCurvePoints, ("##ParticleUseSecondaryColor" + std::to_string(idx)).c_str(), showCurves[idx].colorCurve);
			ImGui::Unindent(20.0f);
		}


		ImGui::Text("Velocity: ");
		ImGui::SameLine();
		ImGui::DragFloat3(("##initialVelocityDragFloat" + std::to_string(idx)).c_str(), glm::value_ptr(p.initialVelocity));
		RenderMultiCurveConfig(idx, selectedObj, true, p.useVelCurve, p.velCurvePoints, ("##CurveVelButton" + std::to_string(idx)).c_str(), showCurves[idx].velCurve);

		ImGui::Indent(20.0f);
		ImGui::Text("Apply Transform Direction: ");
		ImGui::SameLine();
		ImGui::Checkbox(("##ApplyTransformDirection" + std::to_string(idx)).c_str(), &p.useEmitterDirection);
		ImGui::Unindent(20.0f);

		ImGui::Text("Lifetime: ");
		ImGui::SameLine();
		ImGui::DragFloat(("##LifeTimeDragFloat" + std::to_string(idx)).c_str(), &p.lifetime, 0.5f, 0.0f, FLT_MAX);

		ImGui::Text("Spawn Delay: ");
		ImGui::SameLine();
		ImGui::DragFloat(("##pSpawnDelayDragFloat" + std::to_string(idx)).c_str(), &p.spawnDelay, 0.5f, 0.0f, FLT_MAX);

		ImGui::Text("Size: ");
		ImGui::SameLine();
		ImGui::DragFloat(("##PSizeDragFloat" + std::to_string(idx)).c_str(), &p.size, 1.0f, 0.0f, FLT_MAX);
		RenderCurveConfig(idx, selectedObj, true, p.useSizeCurve, p.sizeCurvePoints, ("##CurveSizeButton" + std::to_string(idx)).c_str(), showCurves[idx].sizeCurve);


		ImGui::Text("Fade: ");
		ImGui::SameLine();
		ImGui::Checkbox(("##FadeParticleCheckbox" + std::to_string(idx)).c_str(), &p.fade);
		RenderCurveConfig(idx, selectedObj, p.fade, p.useFadeCurve, p.fadeCurvePoints, ("##CurveFadeButton" + std::to_string(idx)).c_str(), showCurves[idx].fadeCurve);

		ImGui::Text("Use Texture: ");
		ImGui::SameLine();
		ImGui::Checkbox(("##ParticleUseTextureCheckbox" + std::to_string(idx)).c_str(), &p.useTexture);

		if (p.useTexture) {
			int texId = (!ResourceManager::GetLoadedParticleTextures().contains(p.texKey)) ? 0 : ResourceManager::GetLoadedParticleTextures()[p.texKey]->GetId();
			ImGui::ImageButton(("##ParticleTextureImage" + std::to_string(idx)).c_str(), texId, ImVec2(80.0f, 80.0f));
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_INFO")) {
				AssetPayloadType* pData = static_cast<AssetPayloadType*>(payload->Data);
				p.texKey = pData->path;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Text("Rotaion Spread: ");
		ImGui::SameLine();
		if (ImGui::SliderFloat(("##ParticleRotationSpread" + std::to_string(idx)).c_str(), &p.maxSpread, 0.0f, 10.0f)) {
			p.minSpread = -p.maxSpread;
		}

		ImGui::EndChild();
		ImGui::PopID();
		idx++;
	}
}

void BtnSqd::PropertiesMenue::RenderCurveConfig(unsigned int idx, BtnSqd::GameObject& selectedObj, bool show, bool& useCurve, std::vector<glm::vec2>& curvePoints, std::string curveLabel, BtnCurve& curve) {
	if (show) {
		ImGui::Indent(20.0f);
		ImGui::Text("Use Curve: ");
		ImGui::SameLine();
		if (ImGui::Checkbox(("##UseCurveCheckbox" + curveLabel + std::to_string(idx)).c_str(), &useCurve)) {
			if (curvePoints.empty()) {
				curvePoints.push_back(glm::vec2(0.0f));
				curvePoints.push_back(glm::vec2(150.0f, 100.0f));
				UpdateParticleCurves(selectedObj);
			}
		}
		if (useCurve) {
			ImGui::SameLine();
			if (ImGui::Button(("Edit Curve" + curveLabel).c_str())) {
				showCurve = true;
				selectedCurve = &curve;
			}
		}
		ImGui::Unindent(20.0f);
	}
}

void BtnSqd::PropertiesMenue::RenderMultiCurveConfig(unsigned int idx, BtnSqd::GameObject& selectedObj, bool show, bool& useCurve, std::vector<std::vector<glm::vec2>>& curvePoints, std::string curveLabel, BtnCurve& curve) {
	if (show) {
		ImGui::Indent(20.0f);
		ImGui::Text("Use Curve: ");
		ImGui::SameLine();
		if (ImGui::Checkbox(("##UseCurveCheckbox" + curveLabel + std::to_string(idx)).c_str(), &useCurve)) {
			if (curvePoints[0].empty()) {
				for (int i = 0; i < curve.GetCurveCount(); i++) {
					std::vector<glm::vec2> curveP;
					curveP.push_back(glm::vec2(0.0f, 100.0f));
					curveP.push_back(glm::vec2(150.0f, 100.0f));
					curvePoints[i] = curveP;
				}
				UpdateParticleCurves(selectedObj);
			}
		}
		if (useCurve) {
			ImGui::SameLine();
			if (ImGui::Button(("Edit Curve" + curveLabel).c_str())) {
				showCurve = true;
				selectedCurve = &curve;
			}
		}
		ImGui::Unindent(20.0f);
	}
}

void BtnSqd::PropertiesMenue::DrawCurve(BtnSqd::ParticleEmitterComponent& partic) {
	if (selectedCurve) {
		ImGui::SameLine();
		ImGui::Text((selectedCurve->GetName() + ":" + std::to_string(selectedCurve->GetId())).c_str());

		ImGui::Text("Enable Sync: ");
		ImGui::SameLine();
		ImGui::Checkbox("##CurveEditorEnableCurveSync", &selectedCurve->GetSyncCurves());

		ImGuiID curveId = ImGui::GetID("CurveEditor");
		if (ImCurveEdit::Edit(*selectedCurve, ImVec2(325.0f, 150.0f), (unsigned int)curveId)) {
			ProcessCurves(partic);
			partic.particles[0].genCurveTextures = true;//temp
		}
		ImGui::SameLine();
		ImGui::BeginGroup();
		if (ImGui::Button("All##CurveEditorShowAllButton", ImVec2(25.0f, 0.0f)))
			selectedCurve->SetVisibleLayer(-1);
		for (int i = 0; i < selectedCurve->GetCurveCount(); i++) {
			if (ImGui::Button((std::to_string(i) + "##CurveEditorSetCurveIndex" + std::to_string(i)).c_str(), ImVec2(25.0f, 0.0f))) {
				selectedCurve->SetVisibleLayer(i);
			}
			if (selectedCurve->GetSyncCurves()) {
				ImGui::SameLine();
				ImGui::Checkbox(("##CurveEditorSetCurveSync" + std::to_string(i)).c_str(), &selectedCurve->GetSyncCurveIndexFlag(i));
			}
		}
		ImGui::EndGroup();
	}

}

void BtnSqd::PropertiesMenue::ProcessCurves(BtnSqd::ParticleEmitterComponent& partic) {
	unsigned int idx = 0;

	for (auto& p : partic.particles) {
		p.fadeCurvePoints.clear();
		int pointCount = showCurves[idx].fadeCurve.GetPointCount(0);
		for (int i = 0; i < pointCount; i++) {
			ImVec2 point = showCurves[idx].fadeCurve.GetPoints(0)[i];
			p.fadeCurvePoints.push_back(glm::vec2(point.x, point.y));
		}

		p.colorCurvePoints.clear();
		pointCount = showCurves[idx].colorCurve.GetPointCount(0);
		for (int i = 0; i < pointCount; i++) {
			ImVec2 point = showCurves[idx].colorCurve.GetPoints(0)[i];
			p.colorCurvePoints.push_back(glm::vec2(point.x, point.y));
		}

		p.sizeCurvePoints.clear();
		pointCount = showCurves[idx].sizeCurve.GetPointCount(0);
		for (int i = 0; i < pointCount; i++) {
			ImVec2 point = showCurves[idx].sizeCurve.GetPoints(0)[i];
			p.sizeCurvePoints.push_back(glm::vec2(point.x, point.y));
		}

		p.velCurvePoints.clear();
		for (int c = 0; c < showCurves[idx].velCurve.GetCurveCount(); c++) {
			std::vector<glm::vec2>curve;

			pointCount = showCurves[idx].velCurve.GetPointCount(c);
			for (int i = 0; i < pointCount; i++) {
				ImVec2 point = showCurves[idx].velCurve.GetPoints(c)[i];
				curve.push_back(glm::vec2(point.x, point.y));
			}
			p.velCurvePoints.push_back(curve);
		}
		idx++;
	}
}

void BtnSqd::PropertiesMenue::CreateCollider(BtnSqd::GameObject& selectedObj, BtnSqd::ColliderComponent& collider) {
	if (collider.scale.x <= 0) {
		collider.scale.x = 0.1f;
	}
	if (collider.scale.y <= 0) {
		collider.scale.y = 0.1f;
	}
	if (collider.scale.z <= 0) {
		collider.scale.z = 0.1f;
	}
	if (selectedObj.CheckObjectForComponent<PhysicsComponet>()) {
		PhysicsComponet& physicsComp = selectedObj.GetComponent<PhysicsComponet>();
		if (physicsComp.rigidBody)
			physicsComp.rigidBody->detachShape(*collider.pColliderShape);

		btnPhysics.CreateCollider(selectedObj, collider);
		btnPhysics.AddPhysicsObject(selectedObj);
	}
	else {
		btnPhysics.CreateCollider(selectedObj, collider);
	}
}

void BtnSqd::PropertiesMenue::AddScriptPopup(GameObject& selectedObject, ScriptComponent& scriptComp) {
	if (ImGui::BeginPopup("Add Script ##PopupAddScriptWind")) {
		std::filesystem::directory_iterator scriptFolder("Assets/Scripts");

		for (auto scriptFile : scriptFolder) {
			if (scriptFile.path().extension().string() == ".h") {
				std::string scriptName = scriptFile.path().filename().stem().string();
				if (ImGui::Selectable(scriptName.c_str())) {
					showAddScript = false;
					scriptComp.AddScript(currentScene->GetgameObjects()[selectedObject.GetId()].get(), "EngineAssets/Scripts/bin/BtnScripts.dll", scriptName, currentScene.get());
				}
			}
		}

		ImGui::EndPopup();
	}

}

void BtnSqd::PropertiesMenue::UpdateParticleCurves(GameObject& selectedObject) {
	if (selectedObject.CheckObjectForComponent<ParticleEmitterComponent>()) {

		auto toImVec2 = [](const glm::vec2& v) {
			return ImVec2(v.x, v.y);
			};

		ParticleEmitterComponent& partic = selectedObject.GetComponent<ParticleEmitterComponent>();
		showCurves.resize(partic.particles.size());
		selectedCurve = nullptr;

		unsigned int idx = 0;
		for (auto& p : partic.particles) {
			ParticleCurveHolder pch(idx);
			for (const auto& point : p.fadeCurvePoints) {
				pch.fadeCurve.AddPoint(0, toImVec2(point));
			}
			for (const auto& point : p.sizeCurvePoints) {
				pch.sizeCurve.AddPoint(0, toImVec2(point));
			}
			for (const auto& point : p.colorCurvePoints) {
				pch.colorCurve.AddPoint(0, toImVec2(point));
			}

			unsigned int curveIndex = 0;
			for (const auto& points : p.velCurvePoints) {
				for (const auto& point : points) {
					pch.velCurve.AddPoint(curveIndex, toImVec2(point));
				}
				curveIndex++;
			}
			showCurves[idx] = pch;
			p.genCurveTextures = true;
			idx++;
		}
	}
}

void BtnSqd::PropertiesMenue::TexturePicker(std::shared_ptr<Texture>& texture, std::vector<bool>::reference flag, glm::vec3 clearColor, int index) {
	std::string buttonId = "##TexPickerButton-" + index;
	ImGui::BeginChild(buttonId.c_str());

	if (ImGui::ColorButton("##TexturePickerNull", ImVec4(clearColor.x, clearColor.y, clearColor.z, 1.0f), 0, ImVec2(80.0f, 80.0f))) {
		texture = nullptr;
		flag = !flag;
	}

	int i = 0;
	for (auto& [path, tex] : ResourceManager::GetLoadedTextures()) {
		buttonId = "##TexPickerButton-" + path;
		buttonId += index;
		if (ImGui::ImageButton(buttonId.c_str(), tex->GetId(), ImVec2(80.0f, 80.0f))) {
			texture = tex;
			flag = !flag;
		}
		i++;
		if (i % 3 != 0) {
			ImGui::SameLine();
		}
	}
	ImGui::EndChild();
}

void BtnSqd::PropertiesMenue::DrawBtnTextBoxData(std::shared_ptr<BtnWidget> widget) {
	BtnTextBox& text = *static_cast<BtnTextBox*>(widget.get());
	ImGui::BeginChild("DisplayWidgetTypePropertiesWindow",ImVec2(0.0f,0.0f),
					  ImGuiChildFlags_AlwaysAutoResize|ImGuiChildFlags_AutoResizeY|ImGuiChildFlags_Border);
	ImGui::Text("TextBox:");


	ImGui::EndChild();
}
