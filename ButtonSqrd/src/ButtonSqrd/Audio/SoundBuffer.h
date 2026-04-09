#pragma once
#include<cstdint>
#include<AL/al.h>
#include<string>

#include"ButtonSqrd/Core/Logger.h"
#include"AudioLoader.h"

namespace BtnSqd {
	class SoundBuffer {
	public:
		SoundBuffer(const std::string& soundFile);
		~SoundBuffer();
		inline uint32_t GetId() const { return id; }
	private:
		uint32_t id=0;
	};
}