#include "BtnTextureViewer.h"
#include<ButtonSqrd/Core/Logger.h>
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
			showNineSlice = !showNineSlice;
		}

		ImGui::EndGroup();

		ImGui::BeginChild("##BtnTextureViwerContent", ImVec2(0, 0), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);

		ImGui::BeginGroup();
		ImGui::Text(showedTex->GetPath().c_str());

		ImVec2 imagePos = ImGui::GetCursorPos();
		ImGui::Image(showedTex->GetId(), ImVec2(imageSize, imageSize));
		if (showNineSlice) {
			DrawNineSliceEditor(imagePos);
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Text("Texture Settings");
		ImGui::BeginChild("##BtnTextureViwerSettingsWindow", ImVec2(0, imageSize), ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);
		DisplayTexSettings();
		ImGui::EndChild();
		if (ImGui::Button("Apply##BtnTextureViwerContentApplyButton")) {
			showedTex->UpdateSettings(settings);
			showedTex->ApplySettings();
		}
		ImGui::EndGroup();
		ImGui::EndChild();

		ImGui::End();
	}
	void BtnTextureViewer::DisplayTexSettings() {
		ImGui::Text("Texture Type: ");

		if (ImGui::BeginCombo("##TexViewerTexTypeCombo", GetTexType().c_str())) {

			ImGui::EndCombo();
		}
		SetTexSlot();
		SetWrappingMode();
		SetMaxFilter();
		SetMinFilter();
		ImGui::Text("Format: ");
		if (ImGui::BeginCombo("##TexViewerTexFormatCombo", GetTexFormat().c_str())) {

			ImGui::EndCombo();
		}
		ImGui::Text("Data Type: ");
		if (ImGui::BeginCombo("##TexViewerTexDataTypeCombo", GetTexDataType().c_str())) {

			ImGui::EndCombo();
		}
	}
	void BtnTextureViewer::SetWrappingMode() {
		ImGui::Text("Wrapping Mode: ");
		if (ImGui::BeginCombo("##TexViewerTexWrapCombo", GetTexWrapping().c_str())) {
			if (ImGui::Selectable("Repeat##TexViewerTexWrapComboRepeat")) {
				settings.wrappingMode = TextureWrappingInfo::Repeat;
			}
			if (ImGui::Selectable("Mirrored_Repeat##TexViewerTexWrapComboMirrored_Repeat")) {
				settings.wrappingMode = TextureWrappingInfo::Mirrored_Repeat;
			}
			if (ImGui::Selectable("Clamp##TexViewerTexWrapComboClamp")) {
				settings.wrappingMode = TextureWrappingInfo::Clamp;
			}
			if (ImGui::Selectable("Clamp_Border##TexViewerTexWrapComboClamp_Border")) {
				settings.wrappingMode = TextureWrappingInfo::Clamp_Border;
			}
			ImGui::EndCombo();
		}
	}
	void BtnTextureViewer::SetMinFilter() {
		ImGui::Text("Filter Min Mode: ");
		if (ImGui::BeginCombo("##TexViewerMinTexFilterCombo", GetTexMinFilter().c_str())) {
			if (ImGui::Selectable("Linear##TexViewerMinFilterLinear")) {
				settings.texMin = TextureFilterSettings::Linear;
			}
			if (ImGui::Selectable("Linear_Mip_Map##TexViewerMinFilterLinear_Mip_Map")) {
				settings.texMin = TextureFilterSettings::Linear_Mip_Map;
			}
			if (ImGui::Selectable("Near##TexViewerMinFilterNear")) {
				settings.texMin = TextureFilterSettings::Near;
			}
			if (ImGui::Selectable("Near_Linear_mip##TexViewerMinFilterNear_Linear_mip")) {
				settings.texMin = TextureFilterSettings::Near_Linear_mip;
			}
			ImGui::EndCombo();
		}
	}
	void BtnTextureViewer::SetMaxFilter() {
		ImGui::Text("Filter Max Mode: ");
		if (ImGui::BeginCombo("##TexViewerMaxTexFilterCombo", GetTexMaxFilter().c_str())) {
			if (ImGui::Selectable("Linear##TexViewerMaxFilterLinear")) {
				settings.texMax = TextureFilterSettings::Linear;
			}
			if (ImGui::Selectable("Linear_Mip_Map##TexViewerMaxFilterLinear_Mip_Map")) {
				settings.texMax = TextureFilterSettings::Linear_Mip_Map;
			}
			if (ImGui::Selectable("Near##TexViewerMaxFilterNear")) {
				settings.texMax = TextureFilterSettings::Near;
			}
			if (ImGui::Selectable("Near_Linear_mip##TexViewerMaxFilterNear_Linear_mip")) {
				settings.texMax = TextureFilterSettings::Near_Linear_mip;
			}
			ImGui::EndCombo();
		}
	}

	void BtnTextureViewer::SetTexSlot() {
		ImGui::Text("Texture Slot: ");
		if (ImGui::BeginCombo("##TexViewerTexSlotCombo", GetTexSlot().c_str())) {
			if (ImGui::Selectable("Albedo##TexViewerSlotAlbedo")) {
				settings.texSlot = TextureSlot::Albedo;
			}
			if (ImGui::Selectable("Normal##TexViewerSlotNormal")) {
				settings.texSlot = TextureSlot::Normal;
			}
			if (ImGui::Selectable("Texture2##TexViewerSlotTexture2")) {
				settings.texSlot = TextureSlot::Texture2;
			}
			if (ImGui::Selectable("Texture3##TexViewerSlotTexture3")) {
				settings.texSlot = TextureSlot::Texture3;
			}
			if (ImGui::Selectable("Texture4##TexViewerSlotTexture4")) {
				settings.texSlot = TextureSlot::Texture4;
			}
			if (ImGui::Selectable("Texture5##TexViewerSlotTexture5")) {
				settings.texSlot = TextureSlot::Texture5;
			}
			if (ImGui::Selectable("Texture6##TexViewerSlotTexture6")) {
				settings.texSlot = TextureSlot::Texture6;
			}
			if (ImGui::Selectable("Texture7##TexViewerSlotTexture7")) {
				settings.texSlot = TextureSlot::Texture7;
			}
			if (ImGui::Selectable("Texture8##TexViewerSlotTexture8")) {
				settings.texSlot = TextureSlot::Texture8;
			}
			if (ImGui::Selectable("Texture9##TexViewerSlotTexture9")) {
				settings.texSlot = TextureSlot::Texture9;
			}
			if (ImGui::Selectable("Texture10##TexViewerSlotTexture10")) {
				settings.texSlot = TextureSlot::Texture10;
			}
			if (ImGui::Selectable("Texture11##TexViewerSlotTexture11")) {
				settings.texSlot = TextureSlot::Texture11;
			}
			if (ImGui::Selectable("Texture12##TexViewerSlotTexture12")) {
				settings.texSlot = TextureSlot::Texture12;
			}
			if (ImGui::Selectable("Texture13##TexViewerSlotTexture13")) {
				settings.texSlot = TextureSlot::Texture13;
			}
			if (ImGui::Selectable("Texture14##TexViewerSlotTexture14")) {
				settings.texSlot = TextureSlot::Texture14;
			}
			if (ImGui::Selectable("Texture15##TexViewerSlotTexture15")) {
				settings.texSlot = TextureSlot::Texture15;
			}
			if (ImGui::Selectable("Texture16##TexViewerSlotTexture16")) {
				settings.texSlot = TextureSlot::Texture16;
			}
			ImGui::EndCombo();
		}
	}

	void BtnTextureViewer::DrawNineSliceEditor(ImVec2 imagePos) {
		RectSlicePercentages& slices = settings.slices;
		ImVec2 windowPos = ImGui::GetWindowPos();
		HandleLine("##VerticalLeftSliceButton", imagePos, windowPos, ImVec2(imageSize * slices.verticalLeft, 0.0f),
				   ImVec2(imageSize * slices.verticalLeft, imageSize),slices.verticalLeft);

		HandleLine("##VerticalRightSliceButton", imagePos, windowPos, ImVec2(imageSize * slices.verticalRight, 0.0f),
				 ImVec2(imageSize * slices.verticalRight, imageSize),slices.verticalRight);

		HandleLine("##HorizUpSliceButton", imagePos, windowPos, ImVec2(0.0f, imageSize * slices.horizUp),
				 ImVec2(imageSize, imageSize*slices.horizUp),slices.horizUp);
		HandleLine("##HorizDownSliceButton", imagePos, windowPos, ImVec2(0.0f, imageSize * slices.horizDown),
				 ImVec2(imageSize, imageSize * slices.horizDown),slices.horizDown);
	}

	void BtnTextureViewer::HandleLine(std::string label, ImVec2 imagePos, ImVec2 windowPos, ImVec2 point1, ImVec2 point2, float& slice) {

		point1 = ImVec2(imagePos.x + point1.x, imagePos.y + point1.y);
		point2 = ImVec2(imagePos.x + point2.x, imagePos.y + point2.y);

		ImU32 color;
		HandleLineDrag(label,point1, point2,color,slice);
		DrawLine(windowPos, point1, point2,color);
	}

	void BtnTextureViewer::DrawLine(ImVec2 windowPos, ImVec2 point1, ImVec2 point2, ImU32 color) {
		ImVec2 offset = windowPos;
		point1 = ImVec2(offset.x + point1.x, offset.y + point1.y);
		point2 = ImVec2(offset.x + point2.x, offset.y + point2.y);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddLine(point1, point2, color, lineThickness);
	}

	void BtnTextureViewer::HandleLineDrag(std::string label, ImVec2& point1, ImVec2& point2, ImU32& color, float& slice) {
		color = IM_COL32(0, 200, 0, 255);

		ImGui::SetCursorPos(point1);
		ImVec2 buttonSize = ImVec2(lineThickness,imageSize);
		bool isHoriz = false;
		if (point2.x!=point1.x) {
			buttonSize = ImVec2(imageSize, lineThickness);
			isHoriz = true;
		}

		ImGui::InvisibleButton(label.c_str(), buttonSize);
		if (ImGui::IsItemHovered()) {
			color = IM_COL32(0, 255, 0, 255);
		}
		if (ImGui::IsItemActive()) {
			color = IM_COL32(255, 255, 0, 255);
			float netChange = 0.0f;
			if (!isHoriz) {
				netChange += ImGui::GetMouseDragDelta().x;
			}
			else {
				netChange += ImGui::GetMouseDragDelta().y;
			}
			netChange = netChange / imageSize;
			slice += netChange;
			slice = glm::clamp(slice, 0.0f, 1.0f);
			ImGui::ResetMouseDragDelta();
		}
	}


	std::string BtnTextureViewer::GetTexType() {
		static const std::unordered_map<TextureType, std::string> typeMap = {
			{TextureType::Texture_1D,       "Texture_1D"},
			{TextureType::Texture_2D,       "Texture_2D"},
			{TextureType::Texture_3D,       "Texture_3D"},
			{TextureType::CubeMap,          "CubeMap"},
			{TextureType::Texture_2D_Array, "Texture_2D_Array"}
		};

		auto it = typeMap.find(settings.texType);
		return (it != typeMap.end()) ? it->second : std::string();
	}
	std::string BtnTextureViewer::GetTexWrapping() {
		static const std::unordered_map<TextureWrappingInfo, std::string> typeMap = {
			{TextureWrappingInfo::Repeat,          "Repeat"},
			{TextureWrappingInfo::Mirrored_Repeat, "Mirrored_Repeat"},
			{TextureWrappingInfo::Clamp,           "Clamp"},
			{TextureWrappingInfo::Clamp_Border,     "Clamp_Border"}
		};
		auto it = typeMap.find(settings.wrappingMode);
		return (it != typeMap.end()) ? it->second : std::string();
	}

	std::string BtnTextureViewer::GetTexMaxFilter() {
		static const std::unordered_map<TextureFilterSettings, std::string> typeMap = {
			{TextureFilterSettings::Linear,          "Linear"},
			{TextureFilterSettings::Linear_Mip_Map,  "Linear_Mip_Map"},
			{TextureFilterSettings::Near,            "Near"},
			{TextureFilterSettings::Near_Linear_mip, "Near_Linear_mip"}
		};
		auto it = typeMap.find(settings.texMax);
		return (it != typeMap.end()) ? it->second : std::string();
	}

	std::string BtnTextureViewer::GetTexMinFilter() {
		static const std::unordered_map<TextureFilterSettings, std::string> typeMap = {
			{TextureFilterSettings::Linear,          "Linear"},
			{TextureFilterSettings::Linear_Mip_Map,  "Linear_Mip_Map"},
			{TextureFilterSettings::Near,            "Near"},
			{TextureFilterSettings::Near_Linear_mip, "Near_Linear_mip"}
		};
		auto it = typeMap.find(settings.texMin);
		return (it != typeMap.end()) ? it->second : std::string();
	}

	std::string BtnTextureViewer::GetTexFormat() {
		static const std::unordered_map<TexFormat, std::string> typeMap = {
			{TexFormat::RGBA,    "RGBA"},
			{TexFormat::RGB_INT, "RGB_INT"},
			{TexFormat::SINGLE,  "SINGLE"}
		};
		auto it = typeMap.find(settings.texFormat);
		return (it != typeMap.end()) ? it->second : std::string();
	}

	std::string BtnTextureViewer::GetTexDataType() {
		static const std::unordered_map<TextureDataType, std::string> typeMap = {
			{TextureDataType::Color,       "Color"},
			{TextureDataType::SingleColor, "SingleColor"},
			{TextureDataType::Depth,       "Depth"}
		};
		auto it = typeMap.find(settings.texDataType);
		return (it != typeMap.end()) ? it->second : std::string();
	}

	std::string BtnTextureViewer::GetTexSlot() {
		static const std::unordered_map<TextureSlot, std::string> typeMap = {
			{TextureSlot::Albedo, "Albedo"},
			{TextureSlot::Normal, "Normal"},
			{TextureSlot::Texture2, "Texture2"},
			{TextureSlot::Texture3, "Texture3"},
			{TextureSlot::Texture4, "Texture4"},
			{TextureSlot::Texture5, "Texture5"},
			{TextureSlot::Texture6, "Texture6"},
			{TextureSlot::Texture7, "Texture7"},
			{TextureSlot::Texture8, "Texture8"},
			{TextureSlot::Texture9, "Texture9"},
			{TextureSlot::Texture10, "Texture10"},
			{TextureSlot::Texture11, "Texture11"},
			{TextureSlot::Texture12, "Texture12"},
			{TextureSlot::Texture13, "Texture13"},
			{TextureSlot::Texture14, "Texture14"},
			{TextureSlot::Texture15, "Texture15"},
			{TextureSlot::Texture16, "Texture16"}
		};

		auto it = typeMap.find(settings.texSlot);
		return (it != typeMap.end()) ? it->second : std::string();
	}


}