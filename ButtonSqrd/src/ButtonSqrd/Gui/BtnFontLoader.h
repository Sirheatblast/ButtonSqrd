#pragma once
#include"ButtonSqrd/Render/Texture.h"
#include"ButtonSqrd/Core/Logger.h"

#include<memory>
#include <msdf-atlas-gen/msdf-atlas-gen.h>
namespace BtnSqd {
	class BtnFontLoader {
	public:
		std::shared_ptr<Texture> LoadFont(std::string path);
	private:
		
	};
}