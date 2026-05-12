#pragma once
#include"ButtonSqrd/Gui/BtnWidget.h"
#include"ButtonSqrd/Gui/BtnFont.h"
#include"ButtonSqrd/Core/ResourceManager.h"

#include<string>

namespace BtnSqd {
	class BtnTextBox :public BtnWidget {
	public:
		BtnTextBox();
		~BtnTextBox() override;

		Mesh& Draw() override;

		void SetVerts();

	private:
		std::vector<Vertices> UpdateTextVerts();
		float UpdateFontSize();
		float GetMaxWidthGivenSize(float desiredSize);

		std::vector<unsigned int>indices;
		
		unsigned int lastWidth=0;
		unsigned int lastHeight=0;
		float fontSize;
		float border;
		float leterSpacing;
		float maxPerc;
		std::string text;
		bool autoFontSize;

		std::vector<Vertices> verts;
		std::shared_ptr<Mesh> textMesh;
		BtnFont& font;

	};
}