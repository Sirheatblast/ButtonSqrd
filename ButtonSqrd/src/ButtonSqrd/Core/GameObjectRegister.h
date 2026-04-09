#pragma once
#include<string>
#include<vector>
#include<tuple>
#include<map>

#include<entt.hpp>

#include"BtnTransform.h"
#include"ButtonSqrd/Core/CoreComponents.h"

namespace BtnSqd {
	class GameObject;
	class BtnScene;

	class GameObjectRegister {
	public:

		GameObjectRegister(const GameObjectRegister& reg) : scene(reg.scene) {
			gameReg.clear();

			std::unordered_map<entt::entity, entt::entity> entityMap;
			for (auto srcEntity : *reg.gameReg.storage<entt::entity>()) {
				auto dstEntity = gameReg.create();
				entityMap[srcEntity] = dstEntity;
			}

			for (auto srcEntity : *reg.gameReg.storage<entt::entity>()) {
				auto dstEntity = entityMap[srcEntity];

				if (reg.gameReg.any_of<TransformComponent>(srcEntity)) {
					gameReg.emplace<TransformComponent>(dstEntity, reg.gameReg.get<TransformComponent>(srcEntity));
				}
				if (reg.gameReg.any_of<ModelComponent>(srcEntity)) {
					gameReg.emplace<ModelComponent>(dstEntity, reg.gameReg.get<ModelComponent>(srcEntity));
				}
				if (reg.gameReg.any_of<LightComponent>(srcEntity)) {
					gameReg.emplace<LightComponent>(dstEntity, reg.gameReg.get<LightComponent>(srcEntity));
				}
				if (reg.gameReg.any_of<CameraComponent>(srcEntity)) {
					gameReg.emplace<CameraComponent>(dstEntity, reg.gameReg.get<CameraComponent>(srcEntity));
				}
				if (reg.gameReg.any_of<TagComponenet>(srcEntity)) {
					gameReg.emplace<TagComponenet>(dstEntity, reg.gameReg.get<TagComponenet>(srcEntity));
				}
				if (reg.gameReg.any_of<ScriptComponent>(srcEntity)) {
					gameReg.emplace<ScriptComponent>(dstEntity, reg.gameReg.get<ScriptComponent>(srcEntity));
				}
				if (reg.gameReg.any_of<PhysicsComponet>(srcEntity)) {
					gameReg.emplace<PhysicsComponet>(dstEntity, reg.gameReg.get<PhysicsComponet>(srcEntity));
				}
				if (reg.gameReg.any_of<ColliderComponent>(srcEntity)) {
					gameReg.emplace<ColliderComponent>(dstEntity, reg.gameReg.get<ColliderComponent>(srcEntity));
				}
				if (reg.gameReg.any_of<ArmatureComponent>(srcEntity)) {
					gameReg.emplace<ArmatureComponent>(dstEntity, reg.gameReg.get<ArmatureComponent>(srcEntity));
				}
				if (reg.gameReg.any_of<AnimatorComponent>(srcEntity)) {
					gameReg.emplace<AnimatorComponent>(dstEntity, reg.gameReg.get<AnimatorComponent>(srcEntity));
				}
				if (reg.gameReg.any_of<BoneComponent>(srcEntity)) {
					gameReg.emplace<BoneComponent>(dstEntity, reg.gameReg.get<BoneComponent>(srcEntity));
				}
			}
		}
		
		GameObjectRegister& operator=(const GameObjectRegister& reg);

		GameObjectRegister():scene(nullptr){}
		GameObject* CreateNewGameObject();
		GameObject* CreateNewGameObject(uint64_t id);

		void SetScene(BtnScene* currentScene);

		//Returns a list of all components of type
		template<typename ... Component>
		std::vector<std::tuple<Component ...>> GetAllOf(){
			auto view = gameReg.view<Component ...>();
			std::vector<std::tuple<Component ...>> components;

			for (auto gameObject : view) {
				components.push_back(view.get<Component ...>(gameObject));
			}
			return components;
		}

		template<typename... Component>
		std::vector<std::tuple<std::reference_wrapper<Component>...>> GetAllOfRef() {
			auto view = gameReg.view<Component...>();
			std::vector<std::tuple<std::reference_wrapper<Component>...>> components;

			for (auto gameObject : view) {
				components.emplace_back(std::ref(view.get<Component>(gameObject))...);
			}

			return components;
		}

		// Returns all of the Components and the gameobject ID
		template<typename ... Component>
		std::vector<std::tuple<unsigned int, Component ...>> GetAllOfID() {
			auto view = gameReg.view<Component ...>();
			std::vector<std::tuple<unsigned int, Component ...>> components;

			for (auto gameObject : view) {
				components.push_back(std::tuple<unsigned int, Component ...>(static_cast<unsigned int>(gameObject),view.get<Component>(gameObject)...));
			}
			return components;
		}

		entt::registry& GetNative() { return gameReg; }
	private:

		entt::registry gameReg;
		BtnScene* scene;

		friend class GameObject;
	};
}