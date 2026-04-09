#pragma once
#include<glm/glm.hpp>
#include<vector>
#include<string>

#include"ButtonSqrd/Render/Texture.h"

namespace BtnSqd {
	struct ParticleData {
		glm::vec4 position;
		glm::vec4 velocity;
		glm::vec4 orientationQuat;
		glm::vec4 color;
		glm::vec4 tcolor;
		glm::vec4 pData;
		glm::uvec4 pIData;
	};

	struct BtnParticle {
		glm::vec3 initialVelocity = glm::vec3(0.0f);
		glm::vec4 clearColor = glm::vec4(1.0f);
		glm::vec4 transitionColor = glm::vec4(1.0f);

		float lifetime = 1.0f;
		float size = 10.0f;
		float spawnDelay = 1.0f;

		float maxSpread = 1.0f;
		float minSpread = -1.0f;

		bool changeClearColor = false;
		bool changeTColor = false;
		bool fade = true;
		bool enableColorT = false;
		bool useTexture = false;
		std::string texKey = "";
		bool editCurve = false;

		bool useFadeCurve = false;
		bool useColorCurve = false;
		bool useSizeCurve = false;
		bool useVelCurve = false;

		bool useEmitterDirection = false;

		bool genCurveTextures = false;

		std::vector<glm::vec2> fadeCurvePoints;
		std::vector<glm::vec2> colorCurvePoints;
		std::vector<glm::vec2> sizeCurvePoints;
		std::vector<std::vector<glm::vec2>> velCurvePoints;

		BtnParticle() {
			for(int i =0;i<3;i++)
				velCurvePoints.push_back(std::vector<glm::vec2>());
		};
	};

}