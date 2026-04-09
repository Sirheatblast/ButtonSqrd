#pragma once

#include<glm/glm.hpp>
namespace BtnSqd {
	enum LightType {
		DirectionalLight = 0, PointLight, SpotLight
	};

	struct DirectionalLightStruct {		
		glm::vec3 direction;
		float strength;
		glm::vec3 color;
		float padding;
		glm::mat4 lightPositionMat;
	};

	struct PointLightStruct {
		glm::vec3 position;
		float strength;
		glm::vec3 color;
		float padding;
	};
	struct SpotLightStruct {
		glm::vec3 position;
		float strength;
		glm::vec3 color;
		float outerCutOff;
		glm::vec3 direction;
		float cutOff;
		glm::mat4 lightPositionMat;
	};

}