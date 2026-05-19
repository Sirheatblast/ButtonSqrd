#pragma once
#include"ButtonSqrd/Render/Texture.h"
#include"BtnFontLoader.h"

#include<memory>
#include<string>
#include<map>

namespace BtnSqd {
	class BtnFont {
	public:
		BtnFont(std::string path,std::string name,bool& didLoad);
		BtnFont() = default;

		const BtnGlyph* GetGlyph(int code);
		const std::shared_ptr<Texture> GetFontTexture() { return fontTexture; }
		const std::string GetName() { return name; }
	private:
		std::shared_ptr<Texture>fontTexture;
		std::map<int, BtnGlyph>fontGlyphs;
		std::string name;
	};
}