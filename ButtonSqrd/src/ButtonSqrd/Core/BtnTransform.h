#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/quaternion.hpp>

#include"BtnTime.h"
#include"Logger.h"

namespace BtnSqd {
	struct BtnTransform {
	public:
		BtnTransform() :transformMat(glm::mat4(1.0f)),rotationMat(1.0f),localPosition(0.0f),localRotationMat(1.0f), rotation(glm::vec3()),localRotation(glm::vec3()), up(glm::vec3(0.0f, 1.0f, 0.0f)), front(glm::vec3(0.0f, 0.0f, -1.0f)), scale(glm::vec3(1.0)) {}

		void Translate(glm::vec3 newPos) {
			transformMat = glm::translate(transformMat, newPos);
		}

		glm::mat4 GetMatrix() {
			return glm::scale(transformMat * rotationMat, scale);
		}

		glm::mat4& GetTransformMat() {
			return transformMat;
		}
	

		glm::mat4& GetRotationMat() {
			return rotationMat;
		}

		glm::mat4& GetLocalRotationMat() {
			return localRotationMat;
		}

		void MoveTo(glm::vec3 newPos) {

			transformMat[3] = glm::vec4(newPos,1.0);
		}

		void RotateTo(glm::vec3 newRotation) {
			glm::quat rot = glm::angleAxis(glm::radians(newRotation.x), glm::vec3(1.0f, 0.0f, 0.0));
			rot *= glm::angleAxis(glm::radians(newRotation.y),glm::vec3(0.0f,1.0f,0.0));
			rot *= glm::angleAxis(glm::radians(newRotation.z),glm::vec3(0.0f,0.0f,1.0));
			rotationMat = glm::mat4_cast(rot);
			rotation = newRotation;

			front = rot * glm::vec3(0.0f, 0.0f, -1.0f);
			if (glm::length(front) < 1.0f) {
				front += glm::normalize(newRotation)*0.00001f;
			}
		}

		void RotateTo(glm::quat newRotation) {
			rotationMat = glm::mat4_cast(newRotation);
			rotation =  glm::degrees(glm::eulerAngles(newRotation));

			front = newRotation * glm::vec3(0.0f, 0.0f, -1.0f);
			if (glm::length(front) < 1.0f) {
				front += glm::normalize(rotation) * 0.00001f;
			}
		}

		void RotateLocalTo(glm::quat newRotation) {
			localRotationMat = glm::mat4_cast(newRotation);
			localRotation = glm::degrees(glm::eulerAngles(newRotation));

			front = newRotation * glm::vec3(0.0f, 0.0f, -1.0f);
			if (glm::length(front) < 1.0f) {
				front += glm::normalize(rotation) * 0.00001f;
			}
		}

		void Rotate(float angle, glm::vec3 axis) {
			rotation += axis * angle;
			glm::quat rot;
			if (BtnTime::GetDeltaTime()!=0) {
				rot = glm::angleAxis(glm::radians(angle) * BtnTime::GetDeltaTime(), axis);
			}
			else {
				rot = glm::angleAxis(glm::radians(angle), axis);
			}

			rotationMat = glm::mat4_cast(rot);
			front = rot * glm::vec3(0.0f, 0.0f, -1.0f);
			if (glm::length(front) < 1.0f) {
				front += glm::normalize(axis) * 0.00001f;
			}
		}

		void LookAt(glm::vec3 direction) {
			transformMat = glm::lookAt(direction, direction + front, up);
		}

		void Scale(glm::vec3 nScale) {
			scale = nScale;
		}

		glm::vec3 GetRight() {
			return glm::normalize(glm::cross(front, up));
		}

		glm::vec3 GetPosition() {
			return transformMat[3];
		}

		glm::quat GetRotationQ() {
			return glm::quat_cast(rotationMat);
		}

		glm::quat GetLocalRotationQ() {
			return glm::quat_cast(localRotationMat);
		}

		glm::vec3 rotation;
		glm::vec3 localRotation;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 scale;

		glm::vec3 localPosition;
	private:
		glm::mat4 transformMat;
		glm::mat4 rotationMat;
		glm::mat4 localRotationMat;

		friend class SceneSerializer;
	};
}