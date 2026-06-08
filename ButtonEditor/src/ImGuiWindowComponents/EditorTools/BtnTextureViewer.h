#pragma once

#include"ButtonSqrd/Render/Texture.h"

#include<memory>
#include<imgui.h>

namespace BtnSqd {
	class BtnTextureViewer {
	public:
		BtnTextureViewer();
		void SetTexture(std::shared_ptr<Texture> tex);
		void DisplayTexture();
		bool ShouldShow()const { return showViewer; }
		void SetShowViewer(bool show) { showViewer = show; }
	private:
		bool showViewer = false;
		float imageSize = 500.0f;
		std::shared_ptr<Texture> showedTex;
		TextureSettings settings;
	};
}