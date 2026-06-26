#include "AssetDirectory.h"
#include"../Scripting/ScriptManager.h"

namespace BtnSqd {
	AssetDirectory::AssetDirectory() :currentDir("./Assets") {
		TextureSettings texSettings;
		texSettings.wrappingMode = TextureWrappingInfo::Clamp;
		texSettings.texType = TextureType::Texture_2D;

		folderTexture.reset(Texture::Create("./EngineAssets/Textures/FolderImage.png", texSettings));
		fileTexture.reset(Texture::Create("./EngineAssets/Textures/FileIcon.png", texSettings));
		GenerateARTree();

	}
	void AssetDirectory::OnUpdate() {
		ImGui::Begin("Asset directory");

		std::string modeName;
		if (showAssetsLoaded) {
			modeName = "Loaded";
		}
		else {
			modeName = "Files";
		}

		if (ImGui::Button((modeName + "##AssetDirSwitchTypeButton").c_str())) {
			showAssetsLoaded = !showAssetsLoaded;
		}

		if (showAssetsLoaded) {
			if (arTree.GetLevel().parentId >= 0) {
				ImGui::SameLine();
				if (ImGui::Button("Back##AssetDirBackButton")) {
					arTree.MoveUp();
				}
				ImGui::SameLine();
				ImGui::Text(arTree.GetLevel().name.c_str());
			}
			ShowLoadedFiles();
		}
		else {
			if (currentDir != "./Assets") {
				ImGui::SameLine();
				if (ImGui::Button("Back##AssetDirBackButton")) {
					currentDir = currentDir.parent_path();
				}
				ImGui::SameLine();
				ImGui::Text(currentDir.string().c_str());
			}
			ShowRawFiles();
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_INFO")) {
				GameObjectPayload* data = static_cast<GameObjectPayload*>(payload->Data);
				std::string path = "./Assets/SuperGameObjects/";
				SuperGameObject superGameObject(data->gameObject, path);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::End();

		if (viewer.ShouldShow()) {
			viewer.DisplayTexture();
		}
	}
	void AssetDirectory::GetCurrentFileDir() {
		for (auto& p : std::filesystem::directory_iterator(currentDir)) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string nPath = p.path().filename().string();
			std::string uniqueId = "##" + p.path().string();
			if (p.is_directory()) {
				ImGui::BeginGroup();
				if (ImGui::ImageButton(uniqueId.c_str(), folderTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {
					currentDir = p;
				}
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
				ImGui::TextUnformatted(nPath.c_str());
				ImGui::PopTextWrapPos();

				ImGui::EndGroup();
			}
			else {
				if (p.path().extension().string() == ".txt") {
					continue;
				}

				ImGui::BeginGroup();
				if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

				}
				std::string dirType = p.path().parent_path().filename().string();
				CreateDragPayload(p.path().string(), nPath, dirType);
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
				ImGui::TextUnformatted(nPath.c_str());
				ImGui::PopTextWrapPos();

				ImGui::EndGroup();
			}
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}
	void AssetDirectory::CreateDragPayload(const std::string& path, std::string& name, std::string dirType) {
		if (ImGui::BeginDragDropSource()) {
			AssetPayloadType payload;

			strcpy_s(payload.path, path.c_str());
			strcpy_s(payload.name, name.c_str());

			if (dirType == "Models") {
				payload.assetType = AssetRawType::ModelRaw;
			}
			else if (dirType == "Textures") {
				payload.assetType = AssetRawType::TextureRaw;
			}
			else if (dirType == "Shaders") {
				payload.assetType = AssetRawType::ShaderRaw;
			}
			else if (dirType == "SuperGameObjects") {
				payload.assetType = AssetRawType::SuperGameObjectRaw;
			}
			else if (dirType == "Fonts" || path.contains("Fonts")) {
				payload.assetType = AssetRawType::FontRaw;
			}

			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 100.0f);
			ImGui::Text("%s", name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::SetDragDropPayload("ASSET_INFO", &payload, sizeof(AssetPayloadType));
			ImGui::EndDragDropSource();
		}
	}
	void AssetDirectory::AddNewThingsPopUp() {
		if (ImGui::BeginPopup("AddNewThingPopUp", ImGuiPopupFlags_MouseButtonRight)) {
			ImGui::Text("Add");
			ImGui::Indent(10.0f);
			if (ImGui::Selectable("Script")) {
				showScriptCreate = true;
			}
			ImGui::Unindent(10.0f);
			ImGui::EndPopup();
		}
	}
	void AssetDirectory::NewScriptCreateWindow() {
		ImGui::Begin("Create New Script", &showScriptCreate);

		char buffer[26];
		strcpy_s(buffer, newScriptName.c_str());

		ImGui::InputText("##TagInput", buffer, IM_ARRAYSIZE(buffer));
		newScriptName = std::string(buffer);
		if (newScriptName.empty()) {
			newScriptName.append(" ");
		}

		ImGui::SameLine();

		if (ImGui::Button("Create Script")) {
			showScriptCreate = false;
			ScriptManager::CreateScript(newScriptName);
		}
		ImGui::End();
	}

	void AssetDirectory::GenerateARTree() {
		arTree.AddChild("Models", AssetCategory::ModelCat);
		arTree.AddChild("Textures", AssetCategory::None);
		arTree.AddChild("Shaders", AssetCategory::ShaderCat);
		arTree.AddChild("Scenes", AssetCategory::SceneCat);
		arTree.AddChild("Scripts", AssetCategory::ScriptCat);
		arTree.AddChild("Sounds", AssetCategory::SoundCat);
		arTree.AddChild("SuperGameObjects", AssetCategory::SuperGameObjectCat);
		arTree.AddChild("Fonts", AssetCategory::FontCat);

		arTree.MoveDown("Textures");
		arTree.AddChild("Generic", AssetCategory::TextureCat);
		arTree.AddChild("Particle", AssetCategory::ParticleTexCat);
		arTree.AddChild("Skybox", AssetCategory::SkyBoxTexCat);
		arTree.MoveUp();

		CreateLoadedCallbacks();
	}

	void AssetDirectory::CreateLoadedCallbacks() {
		lAssetCallbacks[AssetCategory::ModelCat] = [this]() {
			this->DisplayLoadedModels();
			};
		lAssetCallbacks[AssetCategory::ShaderCat] = [this]() {
			this->DisplayLoadedShaders();
			};
		lAssetCallbacks[AssetCategory::SceneCat] = [this]() {
			this->DisplayLoadedScenes();
			};
		lAssetCallbacks[AssetCategory::ScriptCat] = [this]() {
			this->DisplayLoadedScripts();
			};
		lAssetCallbacks[AssetCategory::SoundCat] = [this]() {
			this->DisplayLoadedSounds();
			};
		lAssetCallbacks[AssetCategory::SuperGameObjectCat] = [this]() {
			this->DisplayLoadedSuperGameObjects();
			};
		lAssetCallbacks[AssetCategory::FontCat] = [this]() {
			this->DisplayLoadedFonts();
			};

		lAssetCallbacks[AssetCategory::TextureCat] = [this]() {
			this->DisplayLoadedTexures();
			};
		lAssetCallbacks[AssetCategory::ParticleTexCat] = [this]() {
			this->DisplayLoadedParticleTextures();
			};
		lAssetCallbacks[AssetCategory::SkyBoxTexCat] = [this]() {
			this->DisplayLoadedSkyBoxTextures();
			};
	}

	void AssetDirectory::ShowRawFiles() {
		ImGui::BeginChild("AssetDirChild", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
		if (showScriptCreate) {
			NewScriptCreateWindow();
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemActive()) {
			Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::MainGUI));
		}

		if (!Application::GetApp()->GetWindow()->GetCursorLock() && ImGui::IsWindowHovered() && Input::IsMouseButtonPressed(MouseCode::Right)) {
			ImGui::OpenPopup("AddNewThingPopUp");
		}
		AddNewThingsPopUp();

		GetCurrentFileDir();
		ImGui::EndChild();
	}

	void AssetDirectory::ShowLoadedFiles() {
		ImGui::BeginChild("AssetDirChild", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemActive()) {
			Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::MainGUI));
		}

		if (!Application::GetApp()->GetWindow()->GetCursorLock() && ImGui::IsWindowHovered() && Input::IsMouseButtonPressed(MouseCode::Right)) {
			ImGui::OpenPopup("AddNewThingPopUp");
		}
		AddNewThingsPopUp();

		for (const auto& [subName, subLevel] : arTree.GetLevel().childrenData) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			AddLoadedFolder("##LoadedResourceSubFolder_" + subName, subName);
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}

		if (lAssetCallbacks.contains(arTree.GetLevel().category)) {
			lAssetCallbacks[arTree.GetLevel().category]();
		}

		ImGui::EndChild();
	}
	void AssetDirectory::AddLoadedFolder(std::string tag, std::string title) {
		ImGui::BeginGroup();
		if (ImGui::ImageButton(tag.c_str(), folderTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {
			arTree.MoveDown(title);
		}
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
		ImGui::TextUnformatted(title.c_str());
		ImGui::PopTextWrapPos();

		ImGui::EndGroup();
	}
	void AssetDirectory::DisplayLoadedModels() {
		for (const auto& [path, model] : ResourceManager::GetLoadedModels()) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string uniqueId = "##LoadedModel_" + path;

			std::string name = GetAssetName(path);

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			CreateDragPayload(path, name, "Models");

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}

	void AssetDirectory::DisplayLoadedTexures() {
		for (const auto& [path,texture]:ResourceManager::GetLoadedTextures()) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string uniqueId = "##LoadedTexture_" + path;

			std::string name = GetAssetName(path);

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), texture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			ManageViewer(texture);

			CreateDragPayload(path,name,"Textures");

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}
	void AssetDirectory::DisplayLoadedSkyBoxTextures() {
		for (const auto& [path, texture] : ResourceManager::GetLoadedSkyboxes()) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string uniqueId = "##LoadedTexture_" + path;

			std::string name = GetAssetName(path);

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			ManageViewer(texture);

			CreateDragPayload(path, name, "Textures");

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}
	void AssetDirectory::DisplayLoadedParticleTextures() {
		for (const auto& [path, texture] : ResourceManager::GetLoadedParticleTextures()) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string uniqueId = "##LoadedTexture_" + path;

			std::string name = GetAssetName(path);

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), texture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			ManageViewer(texture);

			CreateDragPayload(path, name, "Textures");

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}
	void AssetDirectory::ManageViewer(const std::shared_ptr<BtnSqd::Texture>& texture) {
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			viewer.SetShowViewer(true);
			viewer.SetTexture(texture);
		}
	}
	void AssetDirectory::DisplayLoadedSounds() {
		for (const auto& [path,sound]:ResourceManager::GetLoadedSounds()) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string uniqueId = "##LoadedSound_" + path;

			std::string name = GetAssetName(path);

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			CreateDragPayload(path,name,"Sounds");

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}
	void AssetDirectory::DisplayLoadedScenes() {
		for (auto& p : std::filesystem::recursive_directory_iterator("./Assets/Scenes")) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string nPath = p.path().filename().string();
			std::string uniqueId = "##" + p.path().string();
			if (p.path().extension().string() != ".BtnScn") {
				continue;
			}

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}
			CreateDragPayload(p.path().string(), nPath, "Scenes");
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(nPath.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}
	void AssetDirectory::DisplayLoadedScripts() {
		for (auto& p : std::filesystem::recursive_directory_iterator("./Assets/Scripts")) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string nPath = p.path().filename().string();
			std::string uniqueId = "##" + p.path().string();
			std::string extension = p.path().extension().string();
			if (extension != ".cpp"&&extension!=".h") {
				continue;
			}

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			CreateDragPayload(p.path().string(), nPath, "Scripts");
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(nPath.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}
	void AssetDirectory::DisplayLoadedShaders() {
		for (const auto& [path, shader] : ResourceManager::GetLoadedShaders()) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string uniqueId = "##LoadedShader_" + path;

			std::string name = shader->GetShaderName();

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			CreateDragPayload(path, name, "Shaders");

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}

	void AssetDirectory::DisplayLoadedFonts() {
		for (const auto& [path, font] : ResourceManager::GetLoadedFonts()) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string uniqueId = "##LoadedFont_" + path;

			std::string name = path;

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			CreateDragPayload(path, name, "Fonts");

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}
	void AssetDirectory::DisplayLoadedSuperGameObjects() {
		for (const auto& [key, super] : ResourceManager::GetLoadedSuperGameObjects()) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string uniqueId = "##LoadedFont_" + key;

			std::string name = super->GetTag().tag;

			ImGui::BeginGroup();
			if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

			}

			CreateDragPayload(key, name, "SuperGameObjects");

			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
			ImGui::TextUnformatted(name.c_str());
			ImGui::PopTextWrapPos();

			ImGui::EndGroup();
			if ((iconImageSize * 2) + ImGui::GetStyle().ItemSpacing.x + ImGui::GetCursorPosX() < regionAvail) {
				ImGui::SameLine(0.0f, 10.0f);
			}
		}
	}

	std::string AssetDirectory::GetAssetName(std::string path) {
		auto p = std::filesystem::directory_entry(path);
		return p.path().filename().stem().string();
	}
}