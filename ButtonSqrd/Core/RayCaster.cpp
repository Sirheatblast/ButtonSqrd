#include "RayCaster.h"

namespace BtnSqd {
	RayCaster::RayCaster(BtnScene* scene, float maxDistance) {
		currentScene = scene;
		this->maxDistance = maxDistance;
	}

	std::map<ColliderType, std::function<bool(Ray, float&, ColliderComponent, TransformComponent)>> RayCaster::colliderHandles = {
		{ColliderType::BoxCollider,RayCaster::HandleCubicCollider},
		{ColliderType::CapsuleCollider,RayCaster::HandleCapsuleCollider},
		{ColliderType::CircleCollider,RayCaster::HandleSphericalCollider},
		{ColliderType::PlaneCollider,RayCaster::HandlePlaneCollider},
	};

	bool RayCaster::Cast(Ray ray, float maxAngle) {
		for (auto&[transform,collider]:currentScene->GetRegister().GetAllOf<TransformComponent,ColliderComponent>()) {
			if (CheckShouldSkip(ray,transform,collider)) {
				continue;
			}
			float tHit=0.0f;
			
			auto colliderFunc = colliderHandles.find(collider.colliderType);
			if (colliderFunc != colliderHandles.end()) {
				auto handler = colliderFunc->second;
				if (handler(ray, tHit, collider, transform) && tHit <= maxDistance)
					return true;
			}
		}

		return false;
	}

	GameObject RayCaster::CastGameObject(Ray ray, float maxAngle) {
		GameObject found = GameObject();
		float lastHit = FLT_MAX;
		float tHit = 0.0f;
		for (auto [id,collider,transform]:currentScene->GetRegister().GetAllOfID<ColliderComponent,TransformComponent>()) {
			if (CheckShouldSkip(ray, transform, collider)) {
				continue;
			}
			auto colliderFunc = colliderHandles.find(collider.colliderType);
			if (colliderFunc != colliderHandles.end()) {
				auto handler = colliderFunc->second;
				if (handler(ray, tHit, collider, transform) && tHit <= maxDistance && tHit > 0.0f &&tHit<lastHit) {
					lastHit = tHit;
					found = *currentScene->GetgameObjects()[id];
				}
			}
		}
		return found;
	}
	bool RayCaster::CheckShouldSkip(Ray ray, TransformComponent transform, ColliderComponent collider)	{
		if (!collider.isSelectalbe) {
			return true;
		}
		return false;
	}
	bool RayCaster::HandleCubicCollider(Ray ray, float& tHit, ColliderComponent collider, TransformComponent transform) {
		glm::vec3 toCenter = transform.transform.GetPosition() - ray.startPos;
		float closest = -FLT_MAX;
		float farthest = FLT_MAX;
		tHit = FLT_MAX;
		glm::vec3 halfDist = collider.scale;

		glm::vec3 axes[3] = {
			glm::normalize(glm::vec3(transform.transform.GetRotationMat()[0])),
			glm::normalize(glm::vec3(transform.transform.GetRotationMat()[1])),
			glm::normalize(glm::vec3(transform.transform.GetRotationMat()[2]))
		};

		for (int i = 0; i < 3; i++) {
			float projToCenter = glm::dot(axes[i],toCenter);
			float rayProj = glm::dot(axes[i], ray.direction);
			if (fabs(rayProj) > 0.001f) {
				float entryDist = (projToCenter + halfDist[i]) / rayProj;
				float exitDist = (projToCenter - halfDist[i]) / rayProj;
				if (entryDist> exitDist) {
					std::swap(entryDist, exitDist);
				}

				closest = std::max(closest, entryDist);
				farthest = std::min(farthest, exitDist);

				if (closest > farthest) {
					return false;
				}
			}
			else {
				if (-projToCenter - halfDist[i] > 0 || -projToCenter + halfDist[i] < 0) {
					return false;
				}
			}
		}
		tHit = closest;
		return true;
	}
	bool RayCaster::HandleCapsuleCollider(Ray ray, float& tHit, ColliderComponent collider, TransformComponent transform) {
		glm::vec3 capsuleUp = glm::normalize(glm::vec3(transform.transform.GetRotationMat()[1]));
		float halfHeight = collider.halfHeight;

		glm::vec3 pointA = transform.transform.GetPosition() - capsuleUp * halfHeight;
		glm::vec3 pointB = transform.transform.GetPosition() + capsuleUp * halfHeight;
		glm::vec3 capsuleDir = pointB - pointA;
		float capsuleLength = glm::length(capsuleDir);
		glm::vec3 capsuleAxis = capsuleDir / capsuleLength;

		glm::vec3 diff = pointA - ray.startPos;
		float a = glm::dot(capsuleAxis, capsuleAxis);
		float b = glm::dot(capsuleAxis, ray.direction);
		float c = glm::dot(ray.direction, ray.direction);
		float d = glm::dot(capsuleAxis, diff);
		float e = glm::dot(ray.direction, diff);

		float denom = a * c - b * b;
		float s = 0.0f, t = 0.0f;

		if (denom != 0.0f) {
			s = (b * e - c * d) / denom;
			s = glm::clamp(s, 0.0f, capsuleLength);
		}

		t = (b * s + e) / c;

		glm::vec3 closestPointOnSegment = pointA + capsuleAxis * s;
		glm::vec3 closestPointOnRay = ray.startPos + ray.direction * t;

		float distSq = glm::length(closestPointOnSegment - closestPointOnRay);

		if (distSq <= collider.radius * collider.radius) {
			tHit = t;
			return true;
		}

		tHit = FLT_MAX;
		return false;
	}
	bool RayCaster::HandleSphericalCollider(Ray ray, float& tHit, ColliderComponent collider, TransformComponent transform) {
		glm::vec3 oc = ray.startPos - transform.transform.GetPosition();

		float b = glm::dot(oc, ray.direction);
		float c = glm::dot(oc, oc) - collider.radius * collider.radius;

		float discriminant = b * b - c;
		if (discriminant < 0.0f)
			return false;

		float sqrtDisc = sqrt(discriminant);
		float t1 = -b - sqrtDisc;
		float t2 = -b + sqrtDisc;

		if (t1 >= 0.0f) {
			tHit = t1;
			return true;
		}
		else if (t2 >= 0.0f) {
			tHit = t2;
			return true;
		}

		tHit = FLT_MAX;
		return false;
	}

	bool RayCaster::HandlePlaneCollider(Ray ray, float& tHit, ColliderComponent collider, TransformComponent transform) {
		glm::vec3 toCenter = transform.transform.GetPosition() - ray.startPos;
		float closest = -FLT_MAX;
		float farthest = FLT_MAX;
		tHit = FLT_MAX;
		glm::vec3 halfDist = collider.scale;

		glm::vec3 axes[3] = {
			glm::normalize(glm::vec3(transform.transform.GetRotationMat()[0])),
			glm::normalize(glm::vec3(transform.transform.GetRotationMat()[1])),
			glm::normalize(glm::vec3(transform.transform.GetRotationMat()[2]))
		};

		for (int i = 0; i < 3; i++) {
			float projToCenter = glm::dot(axes[i], toCenter);
			float rayProj = glm::dot(axes[i], ray.direction);
			if (fabs(rayProj) > 0.001f) {
				float entryDist = (projToCenter + halfDist[i]) / rayProj;
				float exitDist = (projToCenter - halfDist[i]) / rayProj;
				if (entryDist > exitDist) {
					std::swap(entryDist, exitDist);
				}

				closest = std::max(closest, entryDist);
				farthest = std::min(farthest, exitDist);

				if (closest > farthest) {
					return false;
				}
			}
			else {
				if (-projToCenter - halfDist[i] > 0 || -projToCenter + halfDist[i] < 0) {
					return false;
				}
			}
		}
		tHit = closest;
		return true;
	}
}
