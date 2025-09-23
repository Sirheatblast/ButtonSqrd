#pragma once
#include<glm/glm.hpp>
#include<PxPhysicsAPI.h>
#include<functional>

#include"ButtonSqrd/Render/Model.h"
#include"ButtonSqrd/Render/Light.h"
#include"ButtonSqrd/Render/Camera.h"
#include"ButtonSqrd/Render/BtnArmature.h"

#include"ButtonSqrd/Core/BtnTransform.h"
#include"ButtonSqrd/Scripting/BtnScript.h"

namespace BtnSqd {

	enum ColliderType {
		BoxCollider = 0, CircleCollider, CapsuleCollider, PlaneCollider, CustomCollider
	};

	struct TransformComponent {
		BtnTransform transform;
	};

	struct ModelComponent{
		Model currentModel;
	};

	struct TagComponenet {
		std::string tag;
	};

	struct LightComponent {
		glm::vec3 lightColor;
		float strength;
		LightType lightType;
		float outerCutOff;
		float cutOff;
		glm::mat4 lightMat;

		LightComponent(glm::vec3 lightColor = glm::vec3(1.0),float strength =50.0f,LightType lightType =LightType::DirectionalLight,float outerCutOff =40.0f, float cutOff=35.0f):lightColor(lightColor),lightType(lightType),outerCutOff(outerCutOff),cutOff(cutOff),strength(strength),lightMat(glm::mat4(1.0)) {}
	};

	struct CameraComponent {
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		bool isMainCamera;

		float fov;
		float aspectRatio;
		float nearPlain;
		float farPlain;

		glm::vec3 ScreenToWorldPos(glm::vec2 screenPos);

		CameraComponent(TransformComponent& transform):fov(80.0f),aspectRatio(16.0f/9.0f),nearPlain(0.1f),farPlain(1000.0f),isMainCamera(false) {
			viewMatrix = glm::lookAt(transform.transform.GetPosition(), glm::normalize(transform.transform.GetPosition() - glm::vec3()), transform.transform.up);
			projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlain, farPlain);
		}
		CameraComponent(){}
	};

	using CreateScriptFunc = BtnSqd::BtnScript* (*)();

	struct ScriptEntry {
		const char* name;
		CreateScriptFunc create;
	};

	typedef const ScriptEntry* (*GetScriptsFunc)(size_t*);

	struct ScriptData {
		BtnScript* script;
		bool isEnabled;
		HMODULE dllHandle;
		std::string name;
	};

	class GameObject;
	class SuperGameObject;
	class BtnScene;

	struct ScriptComponent {
		std::vector<ScriptData> scripts;

		~ScriptComponent() {
			//Clear();
		}


		bool AddScript(GameObject* gameObject, const std::string& dllPath, std::string scriptName, BtnScene* scene);
		bool AddScript(SuperGameObject* gameObject, const std::string& dllPath, std::string scriptName);

		bool RemoveScript(std::string scriptName) {
			for (auto scriptItr = scripts.begin(); scriptItr != scripts.end();scriptItr++) {
				auto& sEntry = *scriptItr;
				if (sEntry.name == scriptName) {
					delete sEntry.script;
					FreeLibrary(sEntry.dllHandle);
					scripts.erase(scriptItr);
					return true;
				}
			}
			return false;
		}

		void Clear() {
			for (auto& sEntry:scripts) {
				if (sEntry.script) {
					delete sEntry.script;
				}

				if (sEntry.dllHandle) {
					FreeLibrary(sEntry.dllHandle);
				}
			}
			scripts.clear();
		}

	};
	enum class BtnForce {
		Velocity,
		Impulse,
		Force
	};

	struct PhysicsComponet {
		physx::PxRigidActor* rigidBody = nullptr;
		bool isDynamic = false;
		bool isKinematic = false;
		bool resetComponent = false;

		//Rotation Lock
		bool lockX = false;
		bool lockY = false;
		bool lockZ = false;

		float mass = 1.0f;

		//Non kinematic only
		bool AddVelocity(glm::vec3 direction, BtnForce force);
		//Non kinematic only
		void SetVelocity(glm::vec3 direction);
		//Non kinematic only
		void SetMaxSpeed(float maxSpeed);
		//Non Kinematic only
		glm::vec3 GetVelocity();

		void MoveKinematic(glm::vec3 direction,glm::quat rotation) {
			
			physx::PxTransform pose{ physx::PxVec3(direction.x, direction.y, direction.z),physx::PxQuat(rotation.x,rotation.y,rotation.z,rotation.w)};

			static_cast<physx::PxRigidDynamic*>(rigidBody)->setKinematicTarget(pose);

		}
	};

	struct ColliderComponent {
		float staticFriction=0.5f;
		float dynamicFriction=0.5f;
		float resitution=0.5f;
		bool isSelectalbe = false;
		bool isTrigger = false;

		//Cube and plane
		glm::vec3 scale=glm::vec3(1.0f);
		bool sameAsTransform = true;
		//sphere
		float radius=0.5f;
		float halfHeight=1.0f;


		physx::PxMaterial* physicsMat = nullptr;
		physx::PxShape* pColliderShape = nullptr;
		physx::PxRigidStatic* dActor = nullptr;
		ColliderType colliderType = ColliderType::BoxCollider;
	};

	struct ArmatureComponent {
		BtnArmature armature;
	};
	struct AnimatorComponent {

	};
}