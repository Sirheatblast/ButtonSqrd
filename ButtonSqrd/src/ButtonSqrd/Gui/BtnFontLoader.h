#pragma once
#include"ButtonSqrd/Render/Texture.h"
#include"ButtonSqrd/Core/Logger.h"

#include<memory>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
namespace BtnSqd {
	struct BtnGlyph {
		glm::vec4 uvAxis;
		glm::vec4 glyphSize;
		float advance;
	};

	class BtnFontLoader {
	public:
		std::shared_ptr<Texture> LoadFont(std::string path,std::map<int,BtnGlyph>&btnGlyphs);
	private:
		
	};
}