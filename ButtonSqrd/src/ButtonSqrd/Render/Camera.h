#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"ButtonSqrd/Core/Input.h"
#include"ButtonSqrd/Core/BtnTime.h"
#include"ButtonSqrd/Core/BtnTransform.h"

namespace BtnSqd {

	class Camera {
	public:
		Camera(glm::vec3 startPos, float fov, float aspectRatio, float nearPlain, float farPlain);

		inline glm::mat4x4 GetViewMatrix() { return transform.GetMatrix(); }
		inline glm::mat4x4 GetProjectionMatrix()const { return projectionMatrix; }
		BtnTransform* GetTransform() { return &transform; }

		void SetPosition(glm::vec3 nPos);
		void SetRotation(glm::vec3 nRotation);

		void RecalcViewMat();
	private:

		BtnTransform transform;

		glm::mat4 projectionMatrix;

		bool firstClick;
	};
}