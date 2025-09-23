#include "GameObject.h"

void BtnSqd::GameObject::UpdateChildrenTransforms() {
	BtnTransform transform = GetComponent<TransformComponent>().transform;
	glm::mat4 transformRotaion = transform.GetRotationMat();

	for (auto childItr = children.begin(); childItr != children.end();childItr++) {
		auto& child = *childItr;
		if (child.IsValid()) {
			BtnTransform& childTransform = child.GetComponent<TransformComponent>().transform;
			glm::vec3 localOffset = childTransform.localPosition;
			glm::vec3 rotatedOffset = transform.GetRotationQ() * localOffset;
			glm::vec3 newWorldPos = transform.GetPosition() + rotatedOffset;

			glm::mat4 localRotation = childTransform.GetLocalRotationMat();

			glm::mat4 fullRotation = transformRotaion * localRotation;

			childTransform.MoveTo(newWorldPos);
			childTransform.RotateTo(glm::quat_cast(fullRotation));
		}
	}
}
