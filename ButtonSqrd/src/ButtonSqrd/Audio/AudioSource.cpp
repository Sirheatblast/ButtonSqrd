#include "AudioSource.h"
#include"ButtonSqrd/Core/Logger.h"
namespace BtnSqd {

	AudioSource::AudioSource() {
		alGenSources(1, &id);
	}
	AudioSource::~AudioSource() {

	}
	void AudioSource::SetAudioBuffer(uint32_t bufferId)	{
		alSourcei(id, AL_BUFFER, bufferId);
	}

	void AudioSource::DestroySource() {
		alDeleteSources(1, &id);
	}

	const std::string alErrorToString(ALenum err) {
		switch (err) {
		case AL_NO_ERROR:          return "AL_NO_ERROR: No error";
		case AL_INVALID_NAME:      return "AL_INVALID_NAME: Invalid name parameter";
		case AL_INVALID_ENUM:      return "AL_INVALID_ENUM: Invalid enum parameter";
		case AL_INVALID_VALUE:     return "AL_INVALID_VALUE: Invalid value parameter";
		case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION: Invalid operation";
		case AL_OUT_OF_MEMORY:     return "AL_OUT_OF_MEMORY: Out of memory";
		default:                   return "Unknown OpenAL error";
		}
	}


	void AudioSource::SetPosition(glm::vec3 position) {
		alSource3f(id, AL_POSITION, position.x, position.y, position.z);
		ALenum err = alGetError();
		if (err != AL_NO_ERROR) {
			BTNLOG_ERROR(alErrorToString(err))
		}
	}
}

