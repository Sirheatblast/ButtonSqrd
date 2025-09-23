#include"GameObjectRegister.h"
#include"GameObject.h"
#include"Scene.h"

namespace BtnSqd {
	GameObjectRegister& GameObjectRegister::operator=(const GameObjectRegister& reg) {
		if (this != &reg) {
			gameReg.clear();

			std::unordered_map<entt::entity, GameObject> entityMap;

			for (auto entity = reg.gameReg.storage<entt::entity>()->begin(); entity != reg.gameReg.storage<entt::entity>()->end(); entity++) {
				auto srcEntity = *entity;
				if (reg.gameReg.valid(srcEntity)) {
					std::string name = reg.gameReg.get<TagComponenet>(srcEntity).tag;
					entityMap[srcEntity] = scene->CreateNewGameObject(name, (uint64_t)srcEntity);
				}
			}

			for (auto entity = reg.gameReg.storage<entt::entity>()->begin(); entity != reg.gameReg.storage<entt::entity>()->end(); entity++) {
				auto srcEntity = *entity;

				if (reg.gameReg.any_of<TagComponenet>(srcEntity)) {
					TagComponenet tag = reg.gameReg.get<TagComponenet>(srcEntity);
					entityMap[srcEntity].GetComponent<TagComponenet>() = tag;
				}
				if (reg.gameReg.any_of<TransformComponent>(srcEntity)) {
					TransformComponent transform = reg.gameReg.get<TransformComponent>(srcEntity);
					entityMap[srcEntity].GetComponent<TransformComponent>() = transform;
				}
				if (reg.gameReg.any_of<ModelComponent>(srcEntity)) {
					ModelComponent model = reg.gameReg.get<ModelComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<ModelComponent>();
					entityMap[srcEntity].GetComponent<ModelComponent>() = model;
				}
				if (reg.gameReg.any_of<LightComponent>(srcEntity)) {
					LightComponent light = reg.gameReg.get<LightComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<LightComponent>();
					entityMap[srcEntity].GetComponent<LightComponent>() = light;
				}
				if (reg.gameReg.any_of<CameraComponent>(srcEntity)) {
					CameraComponent camera = reg.gameReg.get<CameraComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<CameraComponent>(entityMap[srcEntity].GetComponent<TransformComponent>());
					entityMap[srcEntity].GetComponent<CameraComponent>() = camera;
				}
				if (reg.gameReg.any_of<ScriptComponent>(srcEntity)) {
					ScriptComponent script = reg.gameReg.get<ScriptComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<ScriptComponent>();
					entityMap[srcEntity].GetComponent<ScriptComponent>() = script;
				}
				if (reg.gameReg.any_of<PhysicsComponet>(srcEntity)) {
					PhysicsComponet physics = reg.gameReg.get<PhysicsComponet>(srcEntity);
					entityMap[srcEntity].AddComponent<PhysicsComponet>();
					entityMap[srcEntity].GetComponent<PhysicsComponet>() = physics;
				}
				if (reg.gameReg.any_of<ColliderComponent>(srcEntity)) {
					ColliderComponent collider= reg.gameReg.get<ColliderComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<ColliderComponent>();
					entityMap[srcEntity].GetComponent<ColliderComponent>() = collider;
				}
			}
		}
		return *this;
	}
	GameObject* GameObjectRegister::CreateNewGameObject(){
		GameObject* newGameObject = new GameObject(gameReg.create(), scene);
		return newGameObject;
	}

	GameObject* GameObjectRegister::CreateNewGameObject(uint64_t id) {
		GameObject* newGameObject = new GameObject(gameReg.create((entt::entity)id), scene);
		return newGameObject;
	}

	void GameObjectRegister::SetScene(BtnScene* currentScene) {
		scene = currentScene;
	}
}