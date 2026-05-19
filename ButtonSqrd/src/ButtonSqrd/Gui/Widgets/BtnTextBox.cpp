#include "BtnTextBox.h"

namespace BtnSqd {

	BtnTextBox::BtnTextBox():font(ResourceManager::GetLoadedFonts().begin()->second) {
		name = "new TextBox";
		text = "Basic Text";
		width = 200.0f;
		height = 100.0f;
		border = 0.0f;
		fontSize = 35.0f;
		maxPerc = 0.95f;
		autoFontSize = false;
		letterSpacing = 0.1f;
		wType = BtnWidgetType::Text;
		backgroundColor = glm::vec4(0.0f);
		SetVerts();
	}

	BtnTextBox::~BtnTextBox() {

	}

	Mesh& BtnSqd::BtnTextBox::Draw() {
		if (lastWidth != width || lastHeight != height) {
			SetVerts();
		}
		return *textMesh;
	}
	void BtnTextBox::SetVerts() {
		verts = UpdateTextVerts();
		lastWidth = width;
		lastHeight = height;
		textMesh = std::make_shared<Mesh>(verts, indices, Material());
	}
	void BtnTextBox::SetText(std::string nText) {
		text = nText;
		SetVerts();
	}
	std::vector<Vertices> BtnTextBox::UpdateTextVerts() {
		std::vector<Vertices> textVerts;
		indices.clear();
		unsigned int vOffset=0;
		glm::vec2 cursor(border,border+fontSize);

		for (const auto& c : text) {
			if (c =='\n') {
				cursor.x = border;
				cursor.y += fontSize;
				continue;
			}
			const BtnGlyph* glyph = font.GetGlyph(c);
			if (glyph) {
				float x0 = cursor.x + glyph->glyphSize.x * fontSize;
				float y0 = cursor.y - glyph->glyphSize.y * fontSize;
				float x1 = cursor.x + glyph->glyphSize.z * fontSize;
				float y1 = cursor.y - glyph->glyphSize.w * fontSize;

				if (x1>width) {
					cursor.x = border;
					cursor.y += fontSize;
				}

				if (y0 > height) {
					continue;
				}

				glm::vec2 uvMin;
				glm::vec2 uvMax;

				uvMin = glm::vec2(glyph->uvAxis.x,glyph->uvAxis.y);
				uvMax = glm::vec2(glyph->uvAxis.z,glyph->uvAxis.w);

				textVerts.push_back({{x0,y0,0.0f},{uvMin.x,uvMin.y}});
				textVerts.push_back({{x1,y0,0.0f},{uvMax.x,uvMin.y}});
				textVerts.push_back({{x1,y1,0.0f},{uvMax.x,uvMax.y}});
				textVerts.push_back({{x0,y1,0.0f},{uvMin.x,uvMax.y}});

				indices.push_back(vOffset + 0);
				indices.push_back(vOffset + 1);
				indices.push_back(vOffset + 2);

				indices.push_back(vOffset + 0);
				indices.push_back(vOffset + 2);
				indices.push_back(vOffset + 3);


				vOffset += 4;
				cursor.x += (glyph->advance+letterSpacing) * fontSize;
			}
			else {
				BTNLOG_WARN("Char: {} does not exist in font", c)
					continue;
			}
		}

		if (autoFontSize) {
			fontSize = UpdateFontSize();
		}

		return textVerts;
	}
	float BtnTextBox::UpdateFontSize() {

		float min = 8.0f;
		float max = 500.0f;
		float currentSize = GetMaxWidthGivenSize(fontSize);
		float best = fontSize;
		float perc = (currentSize / width);
		while (perc < maxPerc || perc > 1.0f) {
			if (max == min || min<8.0f||max<8.0f) {
				break;
			}

			float mid = min + (max - min) / 2.0f;

			currentSize = GetMaxWidthGivenSize(mid);
			if (currentSize == 0.0f) {
				break;
			}

			if (currentSize<=width) {
				best = mid;
				min = mid;
			}
			else {
				max = mid;
			}
			perc = (currentSize / width);
		}
		
		return best;
	}
	float BtnTextBox::GetMaxWidthGivenSize(float desiredSize) {
		float maxLineWidth = 0.0f;
		float currentX = 0.0f;

		for (const auto& c : text) {
			if (c == '\n') {
				if (currentX > maxLineWidth) maxLineWidth = currentX;
				currentX = 0.0f;
				continue;
			}

			const auto& glyph = font.GetGlyph(c);
			if (glyph) {
				currentX += (glyph->advance + letterSpacing) * desiredSize;
			}
		}

		if (currentX > maxLineWidth) {
			maxLineWidth = currentX;
		}
		return maxLineWidth + (border * 2.0f);
	}
}

