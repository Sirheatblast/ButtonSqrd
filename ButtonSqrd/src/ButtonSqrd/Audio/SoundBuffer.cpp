#include "SoundBuffer.h"

namespace BtnSqd {
	SoundBuffer::SoundBuffer(const std::string& soundFile) {
		PCMPackage pcm = AudioLoader::Load(soundFile);

		alGenBuffers(1, &id);
		if (alGetError()!=AL_NO_ERROR) {
			BTNLOG_ERROR("Failed to create sound buffer for:{}",soundFile)
		}

		ALenum format = (pcm.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		alBufferData(id, format, pcm.samples.data(), static_cast<ALsizei>(pcm.samples.size() * sizeof(short)),pcm.sampleRate);
	}
	SoundBuffer::~SoundBuffer()	{
		alDeleteBuffers(1, &id);
	}
}
