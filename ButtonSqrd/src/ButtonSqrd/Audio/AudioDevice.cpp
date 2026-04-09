#include "AudioDevice.h"
namespace BtnSqd {
	AudioDevice::AudioDevice(std::string deviceName) {

		device = alcOpenDevice(deviceName.empty() ? nullptr : deviceName.c_str());
		if (device) {
			context = alcCreateContext(device, nullptr);
			if (context) {
				alcMakeContextCurrent(context);
				if (!alcMakeContextCurrent(context)) {
					alcDestroyContext(context);
					alcCloseDevice(device);
					BTNLOG_ERROR("Failed to set current context")
				}

			}
			else {
				alcCloseDevice(device);
				BTNLOG_ERROR("Failed to make audio context")
			}
		}
		else {
			BTNLOG_ERROR("Failed to open device:{}",deviceName)
		}
	}
	AudioDevice::~AudioDevice()	{
		if (context) {
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(context);
		}
		if (device) {
			alcCloseDevice(device);
		}
	}
	void AudioDevice::UpdateCurrentContext() {
		if (!alcMakeContextCurrent(context)) {
			BTNLOG_ERROR("Failed to set current context")
		}

	}
}
