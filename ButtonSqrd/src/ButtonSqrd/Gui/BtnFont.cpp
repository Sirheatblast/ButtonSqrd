#include"BtnFont.h"

namespace BtnSqd {
	BtnFont::BtnFont(std::string path,bool& didLoad) {
		didLoad = false;
		BtnFontLoader fLoader;
		std::shared_ptr<Texture>fontTex = fLoader.LoadFont(path, fontGlyphs);
		if (!fontTex) {
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