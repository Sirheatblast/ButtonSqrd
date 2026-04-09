#include "AssetDirectory.h"
#include"../Scripting/ScriptManager.h"

namespace BtnSqd {
	AssetDirectory::AssetDirectory():currentDir("./Assets") {
		TextureSettings texSettings;
		texSettings.wrappingMode = TextureWrappingInfo::Clamp;
		texSettings.texType = TextureType::Texture_2D;
		folderTexture.reset(Texture::Create("./EngineAssets/Textures/FolderImage.png",texSettings));
		fileTexture.reset(Texture::Create("./EngineAssets/Textures/FileIcon.png",texSettings));
	}
	void AssetDirectory::OnUpdate()	{
		ImGui::Begin("Asset directory");
		ImGui::BeginChild("AssetDirChild", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
		if (showScriptCreate) {
			NewScriptCreateWindow();
		}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)&&!ImGui::IsAnyItemActive()) {
			Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::MainGUI));
		}

		if (!Application::GetApp()->GetWindow()->GetCursorLock()&&ImGui::IsWindowHovered() && Input::IsMouseButtonPressed(MouseCode::Right)) {
			ImGui::OpenPopup("AddNewThingPopUp");
		}
		AddNewThingsPopUp();

		if (currentDir != "./Assets") {
			if (ImGui::Button("Back")) {
				currentDir = currentDir.parent_path();
			}
		}
		GetCurrentFileDir();
		ImGui::EndChild();
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_INFO")) {
				GameObjectPayload* data = static_cast<GameObjectPayload*>(payload->Data);
				std::string path = "./Assets/SuperGameObjects/";
				SuperGameObject superGameObject(data->gameObject.get(),path);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::End();
	}
	void AssetDirectory::GetCurrentFileDir() {
		for (auto& p : std::filesystem::directory_iterator(currentDir)) {
			float regionAvail = ImGui::GetContentRegionAvail().x;
			std::string nPath = p.path().filename().string();
			std::string uniqueId = "##" + p.path().string();
			if (p.is_directory()) {
				ImGui::BeginGroup();
				if (ImGui::ImageButton(uniqueId.c_str(), folderTexture->GetId(),ImVec2(iconImageSize, iconImageSize),ImVec2(0.0f,1.0f),ImVec2(1.0f,0.0f))) {
					currentDir = p;
				}
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
				ImGui::TextUnformatted(nPath.c_str());
				ImGui::PopTextWrapPos();

				ImGui::EndGroup();
				ImGui::SameLine();
			}
			else {
				ImGui::BeginGroup();
				if (ImGui::ImageButton(uniqueId.c_str(), fileTexture->GetId(), ImVec2(iconImageSize, iconImageSize), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) {

				}
				if (ImGui::BeginDragDropSource()) {
					AssetPayloadType payload;

					strcpy_s(payload.path, p.path().generic_string().c_str());
					strcpy_s(payload.name, nPath.c_str());

					std::string dirType = p.path().parent_path().filename().string();
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

					ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 100.0f);
					ImGui::Text("%s", nPath.c_str());
					ImGui::PopTextWrapPos();

					ImGui::SetDragDropPayload("ASSET_INFO", &payload, sizeof(AssetPayloadType));
					ImGui::EndDragDropSource();
				}
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconImageSize);
				ImGui::TextUnformatted(nPath.c_str());
				ImGui::PopTextWrapPos();

				ImGui::EndGroup();
				if ((iconImageSize*2)+ImGui::GetStyle().ItemSpacing.x+ImGui::GetCursorPosX() <regionAvail) {
					ImGui::SameLine(0.0f,10.0f);
				}
			}
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
}