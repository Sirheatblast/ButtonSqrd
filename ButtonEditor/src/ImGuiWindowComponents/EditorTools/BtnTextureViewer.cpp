#include "BtnTextureViewer.h"

namespace BtnSqd {
	BtnTextureViewer::BtnTextureViewer() {}
	void BtnTextureViewer::SetTexture(std::shared_ptr<Texture> tex) {
		showedTex = tex;
		settings = showedTex->GetSettings();
	}
	void BtnTextureViewer::DisplayTexture() {

		ImGui::Begin("BtnTexureViewer", &showViewer, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize);
		
		ImGui::BeginGroup();

		if (ImGui::Button("9-Slice##BtnTexureViewerNineSliceButton")) {

		}

		ImGui::EndGroup();

		ImGui::BeginChild("##BtnTextureViwerContent",ImVec2(0,0), ImGuiChildFlags_AlwaysAutoResize| ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);
		
		ImGui::BeginGroup();
		ImGui::Text(showedTex->GetPath().c_str());
		ImGui::Image(showedTex->GetId(), ImVec2(imageSize, imageSize));
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Text("Texture Settings");
		ImGui::BeginChild("##BtnTextureViwerSettingsWindow", ImVec2(0, imageSize), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY|ImGuiChildFlags_Border);
		ImGui::Text("Texture Type: ");
		ImGui::Text("Wrapping Mode: ");
		ImGui::Text("Filter Mode: ");
		ImGui::Text("Format: ");
		ImGui::Text("Data Type: ");
		ImGui::EndChild();
		if (ImGui::Button("Apply##BtnTextureViwerContentApplyButton")) {
			showViewer = false;
			showedTex->UpdateSettings(settings);
			showedTex->ApplySettings();
		}
		ImGui::EndGroup();
		ImGui::EndChild();

		ImGui::End();
	}
}