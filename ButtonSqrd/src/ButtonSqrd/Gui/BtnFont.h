#pragma once
#include"ButtonSqrd/Render/Texture.h"
#include"BtnFontLoader.h"

#include<memory>
#include<string>
#include<map>

namespace BtnSqd {
	class BtnFont {
	public:
		BtnFont(std::string path,bool& didLoad);
		BtnFont() = default;

		const BtnGlyph* GetGlyph(int code);
	private:
		std::shared_ptr<Texture>fontTexture;
		std::map<int, BtnGlyph>fontGlyphs;
	};
}