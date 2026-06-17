#pragma once

#include"ButtonSqrd/Render/Texture.h"

#include<memory>
#include<unordered_map>
#include<imgui.h>

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
		void DrawLine(ImVec2 offset,ImVec2 point1,ImVec2 point2);

		bool showViewer = false;
		bool showNineSlice = false;
		float imageSize = 500.0f;
		std::shared_ptr<Texture> showedTex;
		TextureSettings settings;
	};
}