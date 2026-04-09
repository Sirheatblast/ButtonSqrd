#pragma once

#pragma warning(push)
#pragma warning(disable: 4701)
#include <PxPhysicsAPI.h>
#pragma warning(pop)


#include"ButtonSqrd/Core/Logger.h"
#include"ButtonSqrd/Core/Scene.h"
#include"ButtonSqrd/Core/BtnTime.h"

namespace BtnSqd {
	//Is wrapper around PhysX
	class BtnPhysics {
	public:
		BtnPhysics(std::shared_ptr<BtnScene>& scene):currentScene(scene){}
		~BtnPhysics();

		bool Init();
		void Update();
		bool ShutDown();

		void OnSceneLoad();

		void AddPhysicsObject(GameObject& gameObject);
		void CreateCollider(GameObject& gameObject,ColliderComponent& collider);
		void Reset();
		void ReInit();
		void DisplayColliders();
		bool RemoveObject(GameObject& gameObject);
		
		static physx::PxVec3 glmToPxVec3(const glm::vec3& v) { return physx::PxVec3(v.x, v.y, v.z); }
		static physx::PxQuat glmToPxQuat(const glm::quat& q) { return physx::PxQuat(q.x, q.y, q.z, q.w); }
		static glm::quat pxToGlmQuat(const physx::PxQuat& q) { return glm::quat(q.w, q.x, q.y, q.z); }
		static glm::vec3 pxToGlmVec3(const physx::PxVec3& v) { return glm::vec3(v.x, v.y, v.z); }

	private:
		static std::unordered_map<physx::PxActor*, std::shared_ptr<GameObject>> physicsGameObjects;

		//Only works with models with one mesh
		physx::PxShape* CreateCustomCollider(ColliderComponent& collider,GameObject gameObject);

		class UserErrorCallback : public physx::PxErrorCallback	{
		public:

			void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override {
				std::string fullMessage = std::string(message) + " " + std::string(file) + " " + std::to_string(line);
				switch (code) {
				case physx::PxErrorCode::Enum::eABORT:
					BTN_CORE_LOG_ERROR(fullMessage);
					break;
				case physx::PxErrorCode::Enum::eDEBUG_INFO:
					BTN_CORE_LOG_INFO(fullMessage);
					break;
				case physx::PxErrorCode::Enum::eDEBUG_WARNING:
					BTN_CORE_LOG_WARN(fullMessage);
					break;
				case physx::PxErrorCode::Enum::eINTERNAL_ERROR:
					BTN_CORE_LOG_ERROR(fullMessage);
					break;
				case physx::PxErrorCode::Enum::eINVALID_OPERATION:
					BTN_CORE_LOG_ERROR(fullMessage);
					break;
				case physx::PxErrorCode::Enum::eINVALID_PARAMETER:
					BTN_CORE_LOG_ERROR(fullMessage);
					break;
				case physx::PxErrorCode::Enum::eMASK_ALL:
					BTNLOG_ERROR(fullMessage);
					break;
				case physx::PxErrorCode::Enum::eNO_ERROR:
					BTN_CORE_LOG_INFO(fullMessage);
					break;
				case physx::PxErrorCode::Enum::eOUT_OF_MEMORY:
					BTN_CORE_LOG_ERROR(fullMessage);
					break;
				case physx::PxErrorCode::Enum::ePERF_WARNING:
					BTN_CORE_LOG_WARN(fullMessage);
				}
			}
		};
		class ColliderTriggerCallback : public physx::PxSimulationEventCallback {
			void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
			void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 count) override;
			
			void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) override {}
			void onWake(physx::PxActor**, physx::PxU32) override {}
			void onSleep(physx::PxActor**, physx::PxU32) override {}
			void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override{}
			friend class BtnPhysics;
		};

		UserErrorCallback errorCallback;
		physx::PxDefaultAllocator defaultAllocator;
		physx::PxTolerancesScale toleranceScale;
		physx::PxDefaultCpuDispatcher* defaultCpuDispatcher = nullptr;

		physx::PxFoundation* foundation = nullptr;
		physx::PxPhysics* physics = nullptr;
		physx::PxSceneDesc* sceneDesc=nullptr;

		physx::PxScene* pScene = nullptr;
		std::shared_ptr<BtnScene>& currentScene;
	};
}