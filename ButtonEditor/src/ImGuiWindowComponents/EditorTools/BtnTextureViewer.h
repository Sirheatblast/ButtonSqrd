#pragma once

#include"ButtonSqrd/Render/Texture.h"

#include<memory>
#include<unordered_map>
#include<imgui.h>
#include<glm/glm.hpp>
#include<ButtonSqrd/Core/Input.h>

namespace BtnSqd {
	class BtnTextureViewer {
	public:
		BtnTextureViewer();
		void SetTexture(std::shared_ptr<Texture> tex);
		void DisplayTexture();
		void DisplayTexSettings();
		bool ShouldShow()const { return showViewer; }
		void SetShowViewer(bool show) { showViewer = show; }
	private:
		std::string GetTexType();
		std::string GetTexWrapping();
		std::string GetTexMaxFilter();
		std::string GetTexMinFilter();
		std::string GetTexFormat();
		std::string GetTexDataType();
		std::string GetTexSlot();

		void SetWrappingMode();
		void SetMinFilter();
		void SetMaxFilter();
		void SetTexSlot();

		void DrawNineSliceEditor(ImVec2 imagePos);
		void HandleLine(std::string label,ImVec2 imagePos,ImVec2 windowPos, ImVec2 point1, ImVec2 point2,float& slice);
		void HandleLineDrag(std::string label,ImVec2& point1, ImVec2& point2, ImU32& color, float& slice);
		void DrawLine(ImVec2 windowPos, ImVec2 point1,ImVec2 point2, ImU32 color);

		bool showViewer = false;
		bool showNineSlice = false;
		float imageSize = 500.0f;
		float lineThickness = 6.0f;
		glm::vec2 lastMousePos = glm::vec2(0.0f);
		std::shared_ptr<Texture> showedTex;
		TextureSettings settings;
	};
}