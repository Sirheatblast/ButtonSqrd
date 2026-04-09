#pragma once

#include <glm/glm.hpp>
#include<functional>
#include<map>

#include"Scene.h"
#include"GameObject.h"

namespace BtnSqd {
	struct Ray {
		glm::vec3 startPos;
		glm::vec3 direction;
	};

	class RayCaster {
	public:
		RayCaster(BtnScene* scene,float maxDistance=100.0f);
		bool Cast(Ray ray, float maxAngle=100.0f);
		GameObject CastGameObject(Ray ray, float maxAngle = 100.0f);
	private:
		static std::map<ColliderType, std::function<bool(Ray, float&, ColliderComponent, TransformComponent)>> colliderHandles;

		bool CheckShouldSkip(Ray ray,TransformComponent transform,ColliderComponent collider);

		static bool HandleCubicCollider(Ray ray, float& tHit,ColliderComponent collider, TransformComponent transform);
		static bool HandleCapsuleCollider(Ray ray, float& tHit, ColliderComponent collider, TransformComponent transform);
		static bool HandleSphericalCollider(Ray ray, float& tHit, ColliderComponent collider, TransformComponent transform);
		static bool HandlePlaneCollider(Ray ray, float& tHit, ColliderComponent collider, TransformComponent transform);

		BtnScene* currentScene;
		float maxDistance=100.0f;
	};
}