#pragma once

#include<vector>
#include<string>

#include"ButtonSqrd/Core/Logger.h"

namespace BtnSqd {
	struct PCMPackage {
		std::vector<short>samples;
		int channels=0;
		int sampleRate=0;
	};
	class AudioLoader {
	public:
		static PCMPackage Load(const std::string& soundFile);
	};
}

