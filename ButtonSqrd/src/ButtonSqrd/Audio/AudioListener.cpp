#include "AudioListener.h"
#include"ButtonSqrd/Core/BtnTime.h"
#include"ButtonSqrd/Core/Logger.h"

namespace BtnSqd {
	AudioListener::AudioListener() {
		position = glm::vec3(0.0f);
		oldPos = glm::vec3(0.0f);
		velocity = glm::vec3(0.0f);
		orientation[0] = glm::vec3(0, 0, -1);
		orientation[1] = glm::vec3(0, 1, 0);
		Update();
	}

	AudioListener::~AudioListener() {

	}
	void AudioListener::Update() {
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		alListenerfv(AL_ORIENTATION, glm::value_ptr(orientation));
	}
	void AudioListener::SetPosition(const glm::vec3& nPos)	{
		position = nPos;
	}
	void AudioListener::SetVelocity(const glm::vec3& nPos)	{
		glm::vec3 vel =glm::normalize((nPos - oldPos) / BtnTime::GetDeltaTime());
		oldPos = nPos;
		velocity = vel;
	}
	void AudioListener::SetOrientation(const glm::vec3& nForward, const glm::vec3& nUp)	{
		orientation[0] = nForward;
		orientation[1] = nUp;
	}
}