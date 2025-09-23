#pragma once

#include"ButtonSqrd/Render/Model.h"
#include"ButtonSqrd/Render/Light.h"
#include"ButtonSqrd/Render/Camera.h"
#include"ButtonSqrd/Core/CoreComponents.h"
#include"ButtonSqrd/Core/ResourceManager.h"

#include"GameObjectRegister.h"

#include<vector>
#include<map>
#include<memory>

namespace BtnSqd {

	class GameObject;
	class SuperGameObject;
	class BtnPhysics;
	class BtnScene {
	public:

		BtnScene(const BtnScene& copyScene);

		BtnScene(std::string name = "New Scene") :sceneName(name) {
			gameReg.SetScene(this);
		}

		BtnScene& operator=(const BtnScene& copyScene);

		static std::shared_ptr<BtnScene> CopyToShared(std::shared_ptr<BtnScene> original) {
			std::shared_ptr<BtnScene> copy;
			copy.reset(new BtnScene());
			*copy = *original;
			return copy;
		}

		void UnLoadScene() {
			gameObjects.clear();

			for (auto& [id,comp] : gameReg.GetAllOfID<TransformComponent>()) {
				gameReg.GetNative().destroy(static_cast<entt::entity>(id));
			}
		}

		void SetName(std::string name) {
			sceneName = name;
		}

		GameObject& CreateNewGameObject();
		GameObject& CreateNewGameObject(SuperGameObject* gTemplate);
		GameObject& CreateNewGameObject(std::string objectName);
		GameObject& CreateNewGameObject(std::string objectName, uint64_t gameObjId);

		GameObject GetActiveCamera();

		std::map<uint32_t, std::shared_ptr<GameObject>> GetgameObjects()const { return gameObjects; }
		void ClearGameObjects();
		void DeleteGameObject(GameObject* gameObject);

		std::string GetName()const { return sceneName; }
		GameObjectRegister& GetRegister() { return gameReg; }
		std::shared_ptr<Texture>& GetSkyboxTexture() const { return ResourceManager::GetLoadedSkyboxes()[skyboxId]; }
		void SetSkyboxTexture(std::string texturePath) {
			auto& skyboxes = ResourceManager::GetLoadedSkyboxes();
			if (!skyboxes[texturePath]) {
				TextureSettings texSettings;
				texSettings.texType = TextureType::CubeMap;
				skyboxes[texturePath] = std::shared_ptr<Texture>(Texture::Create(texturePath, texSettings));
			}
			skyboxId = texturePath;
		}

		void OnRuntimeCreate();
		void OnRuntimeStart();
		void OnRuntimeUpdate();
		void OnRuntimePhysicsUpdate();
		void OnRuntimeEnd();

		//Needs the gameObject id from the new camera
		void SetActiveCamera(uint64_t cameraId);
		void SetPhysics(BtnPhysics* pInstance);
	private:
		uint64_t activeCameraId = 0;

		std::string sceneName;
		std::string skyboxId;
		std::map<uint32_t, std::shared_ptr<GameObject>>gameObjects;
		BtnPhysics* physics=nullptr;

		GameObjectRegister gameReg;

		friend class GameObject;
		friend class SceneSerializer;
	};
}