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

		Mesh& Draw(std::shared_ptr<Shader> shader) override;

		void SetVerts();

		bool& GetAutoSize() { return autoFontSize; }
		std::string GetText()const {return text;}
		void SetText(std::string nText);

		BtnFont& GetFont() { return font; }
		float& GetFontSize() { return fontSize; }
		float& GetBorderRef() { return border; }
		float& GetLetterSpacing() { return letterSpacing; }
		glm::vec4& GetBackgroundColor() { return backgroundColor; }
		glm::vec4& GetColorRef() { return color; }

	private:
		std::vector<Vertices> UpdateTextVerts();
		float UpdateFontSize();
		float GetMaxWidthGivenSize(float desiredSize);

		std::vector<unsigned int>indices;
		glm::vec4 backgroundColor;
		unsigned int lastWidth=0;
		unsigned int lastHeight=0;
		float fontSize;
		float letterSpacing;
		float maxPerc;
		std::string text;
		bool autoFontSize;

		std::vector<Vertices> verts;
		std::shared_ptr<Mesh> textMesh;
		BtnFont font;

	};
}