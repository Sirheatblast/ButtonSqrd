#include"BtnFont.h"

namespace BtnSqd {
	BtnFont::BtnFont(std::string path, std::string fName,bool& didLoad) {
		didLoad = false;
		name = fName;
		BtnFontLoader fLoader;
		fontTexture = fLoader.LoadFont(path, fontGlyphs);
		if (!fontTexture) {
			return;
		}
		didLoad = true;
	}
	const BtnGlyph* BtnFont::GetGlyph(int code) {
		if (fontGlyphs.contains(code)) {
			return &fontGlyphs[code];
		}
		return nullptr;
	}
}