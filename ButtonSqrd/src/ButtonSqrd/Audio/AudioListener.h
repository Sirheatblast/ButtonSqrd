#pragma once

#include<AL/al.h>
#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace BtnSqd {
	class AudioListener {
	public:
		AudioListener();
		~AudioListener();
		void Update();

		void SetPosition(const glm::vec3& nPos);
		void SetVelocity(const glm::vec3& nVelocity);
		void SetOrientation(const glm::vec3& nForward, const glm::vec3& nUp);
	private:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 oldPos;
		glm::mat2x3 orientation;
	};
}