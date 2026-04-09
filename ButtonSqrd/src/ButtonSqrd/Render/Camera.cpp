#include"Camera.h"
namespace BtnSqd {
	//Not supported class do not use
	Camera::Camera(glm::vec3 startPos, float fov,float aspectRatio,float nearPlain,float farPlain):firstClick(true){
		transform.GetPosition() = startPos;
		projectionMatrix = glm::perspective(glm::radians(fov),aspectRatio,nearPlain,farPlain);
	}
	void Camera::SetPosition(glm::vec3 nPos){
		transform.GetPosition() = nPos;
		RecalcViewMat();
	}

	void Camera::SetRotation(glm::vec3 nRotation){

	}
	void Camera::RecalcViewMat(){
	}
}