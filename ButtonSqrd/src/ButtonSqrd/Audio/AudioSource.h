#pragma once
#include<AL/al.h>
#include<cstdint>
#include<glm/glm.hpp>

#include"ButtonSqrd/Core/BtnTime.h"

namespace BtnSqd {
    struct AudioBufferInfo {
        unsigned int id=0;
        bool isLooping = false;
        float gain = 1.0f;
        float pitch = 1.0f;
    };

	class AudioSource {
	public:
		AudioSource();
		~AudioSource();

		void SetAudioBuffer(uint32_t bufferId);
        void DestroySource();

		void Play() { alSourcePlay(id); }
		void Stop() { alSourceStop(id); }
		void Pause() { alSourcePause(id); }
		void Rewind() { alSourceRewind(id); }

        void SetLooping(const bool& loop) {
            alSourcei(id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        }
        void SetGain(float gain) {
            alSourcef(id, AL_GAIN, gain);
        }
        void SetPitch(float pitch) {
            alSourcef(id, AL_PITCH, pitch);
        }
        void SetPosition(glm::vec3 position);
        void SetVelocity(glm::vec3 nPos) {
            glm::vec3 vel = glm::normalize((nPos - oldPos) / BtnTime::GetDeltaTime());
            oldPos = nPos;
            alSource3f(id, AL_VELOCITY, vel.x, vel.y, vel.z);
        }

        bool IsPlaying() const {
            ALint state;
            alGetSourcei(id, AL_SOURCE_STATE, &state);
            return state == AL_PLAYING;
        }

        unsigned int GetID()const { return id; }

	private:
		unsigned int id=0;
        glm::vec3 oldPos = glm::vec3(0.0f);
	};
}