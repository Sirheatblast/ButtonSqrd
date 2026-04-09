#pragma once
#include"ButtonSqrd/Core/Logger.h"

#include<AL/al.h>
#include<AL/alc.h>
#include<string>

namespace BtnSqd {
	class AudioDevice {
	public:
		AudioDevice(std::string deviceName = "");
		~AudioDevice();
		void UpdateCurrentContext();
	private:
		ALCdevice* device = nullptr;
		ALCcontext* context =nullptr;
	};
}