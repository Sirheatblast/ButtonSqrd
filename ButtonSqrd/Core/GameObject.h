#pragma once
#include"Scene.h"
#include"CoreComponents.h"

#include<entt.hpp>
#include<typeinfo>

namespace BtnSqd {
	class GameObject {
	public:
		//Doesn't work unless you have a scene
		GameObject(entt::entity gameObj=entt::null, BtnScene* scene = nullptr) :gameObject(gameObj),parent(nullptr), scene(scene), isChild(false), showChildren(false) {}

		bool IsActive() const {
			return gameObject != entt::null;
		}

		void AddChild(GameObject& newChild) {
			scene->GetgameObjects()[newChild.GetId()]->isChild = true;
			scene->GetgameObjects()[newChild.GetId()]->parent = scene->GetgameObjects()[this->GetId()].get();
			scene->GetgameObjects()[this->GetId()]->children.push_back(newChild);
		}

		void RemoveChild(GameObject& oldChild) {
			oldChild.isChild = false;
			oldChild.parent = nullptr;
			for (auto itr = children.begin(); itr != children.end();itr++) {
				if (itr->gameObject == oldChild.gameObject) {
					children.erase(itr);
					break;
				}
			}
		}

		void DestroyGameObject() {
			scene->DeleteGameObject(this);
		}

		template<typename ComponentType>
		ComponentType& AddComponent() {
			if (!CheckObjectForComponent<ComponentType>()) {
				scene->gameReg.GetNative().emplace<ComponentType>(gameObject);
			}
			return GetComponent<ComponentType>();
		}

		template<typename ComponentType,typename Args>
		ComponentType& AddComponent(Args args) {
			scene->gameReg.GetNative().emplace<ComponentType>(gameObject,args);
			return GetComponent<ComponentType>();
		}

		template<typename ComponentType>
		void RemoveComponent() {
			scene->gameReg.GetNative().remove<ComponentType>(gameObject);
		}

		template<typename ComponentType>
		ComponentType& GetComponent() {
			if (!scene->gameReg.GetNative().all_of<ComponentType>(gameObject)) {
				std::string errorString = "GameObject doesn't have Component type: ";
				errorString.append(std::string(typeid(ComponentType).name()));
				BTNLOG_ERROR(errorString);
			}
			return scene->gameReg.GetNative().get<ComponentType>(gameObject);
		}

		template<typename ComponentType>
		bool CheckObjectForComponent() {
			return scene->gameReg.GetNative().any_of<ComponentType>(gameObject);
		}

		template<typename ComponentType>
		auto getAllComponentsOfType() {
			return scene->gameReg.GetNative().view<ComponentType>(gameObject);
		}

		uint32_t GetId()const { return static_cast<uint32_t>(gameObject); }
		bool IsValid() const {
			if (this==nullptr||!scene || gameObject == entt::null) {
				return false;
			}

			const auto& registry = scene->GetRegister().GetNative();
			return registry.valid(gameObject);
		}


		GameObject* GetParent()const { return parent; }

		bool IsChild()const { return isChild; }
		bool GetShowChildren()const { return showChildren; }

		void ChangeShowChildren() { showChildren = !showChildren; }
		void UpdateChildrenTransforms();
		std::vector<GameObject> GetChildren()const { return children; }
	private:
		entt::entity gameObject;
		std::vector<GameObject>children;
		GameObject* parent;
		BtnScene* scene;
		bool isChild;
		bool showChildren;
	};
}