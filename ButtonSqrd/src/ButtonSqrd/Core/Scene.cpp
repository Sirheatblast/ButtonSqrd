#include "Scene.h"
#include"GameObject.h"
#include"SuperGameObject.h"
#include"CoreComponents.h"
#include"ButtonSqrd/Physics/BtnPhysics.h"

namespace BtnSqd {
	BtnScene::BtnScene(const BtnScene& copyScene) {
		sceneName = copyScene.sceneName;
		skyboxId = copyScene.skyboxId;
		gameObjects = copyScene.gameObjects;
		gameReg = copyScene.gameReg;
		physics = copyScene.physics;
		for (auto& [id, gameObj] : copyScene.GetgameObjects()) {
			if (gameObj->CheckObjectForComponent<CameraComponent>()) {
				if (gameObj->GetComponent<CameraComponent>().isMainCamera) {
					SetActiveCamera(id);
				}
			}
			for (auto& child : gameObj->GetChildren()) {
				gameObjects[id]->AddChild(*gameObjects[child.GetId()]);
			}
		}
	}

	BtnScene& BtnScene::operator=(const BtnScene& copyScene) {
		if (this != &copyScene) {
			sceneName = copyScene.sceneName;
			skyboxId = copyScene.skyboxId;
			gameObjects = copyScene.gameObjects;
			physics = copyScene.physics;

			gameReg = copyScene.gameReg;
			for (auto& [id, gameObj] : copyScene.GetgameObjects()) {
				if (!gameObj)continue;
				if (gameObj->CheckObjectForComponent<CameraComponent>()) {
					const auto& camera = gameObj->GetComponent<CameraComponent>();
					if (camera.isMainCamera) {
						SetActiveCamera(id);
					}
				}

				for (auto& child : gameObj->GetChildren()) {
					gameObjects[id]->AddChild(*gameObjects[child.GetId()]);
				}
			}
		}
		return *this;
	}

	GameObject& BtnScene::CreateNewGameObject() {
		GameObject* newObject = gameReg.CreateNewGameObject();
		std::string defName = "GameObject";
		defName += std::to_string(newObject->GetId());
		newObject->AddComponent<TagComponenet>(defName);
		newObject->AddComponent<TransformComponent>();
		gameObjects[newObject->GetId()] = std::shared_ptr<GameObject>(newObject);
		return *gameObjects[newObject->GetId()].get();
	}
	GameObject& BtnScene::CreateNewGameObject(SuperGameObject* gTemplate) {
		GameObject* newObject = gameReg.CreateNewGameObject();
		newObject->AddComponent<TagComponenet>(gTemplate->GetTag().tag + std::to_string(newObject->GetId()));
		TransformComponent& transform = newObject->AddComponent<TransformComponent>();
		transform.transform.Scale(gTemplate->GetScale());

		SuperFlags flags = gTemplate->GetFlags();

		if (flags.hasModel) {
			ModelComponent& model = newObject->AddComponent<ModelComponent>();
			model = gTemplate->GetModel();
		}
		if (flags.hasLight) {
			LightComponent& light = newObject->AddComponent<LightComponent>();
			light = gTemplate->GetLight();
		}
		if (flags.hasCamera) {
			CameraComponent& camera = newObject->AddComponent<CameraComponent>(transform);
			camera.aspectRatio = gTemplate->GetCamera().aspectRatio;
			camera.farPlain = gTemplate->GetCamera().farPlain;
			camera.fov = gTemplate->GetCamera().fov;
			camera.isMainCamera = gTemplate->GetCamera().isMainCamera;
			camera.nearPlain = gTemplate->GetCamera().nearPlain;
			camera.projectionMatrix = gTemplate->GetCamera().projectionMatrix;
			camera.viewMatrix = gTemplate->GetCamera().viewMatrix;
		}
		if (flags.hasScript) {
			ScriptComponent& script = newObject->AddComponent<ScriptComponent>();

			std::string dllPath = "./EngineAssets/Scripts/bin/BtnScripts.dll";//temp
			for (auto scriptData : gTemplate->GetScript().scripts) {
				script.AddScript(newObject, dllPath, scriptData.name, this);
			}

			for (auto spt : script.scripts) {
				spt.script->SetGameObject(newObject);

			}
		}
		if (flags.hasPhysics) {
			PhysicsComponet& physics = newObject->AddComponent<PhysicsComponet>();
			physics.isDynamic = gTemplate->GetPhysics().isDynamic;
			physics.isKinematic = gTemplate->GetPhysics().isKinematic;

			physics.lockX = gTemplate->GetPhysics().lockX;
			physics.lockY = gTemplate->GetPhysics().lockY;
			physics.lockZ = gTemplate->GetPhysics().lockZ;

			physics.mass = gTemplate->GetPhysics().mass;
		}
		if (flags.hasCollider) {
			ColliderComponent& collider = newObject->AddComponent<ColliderComponent>();
			collider = gTemplate->GetCollider();
		}

		gameObjects[newObject->GetId()] = std::shared_ptr<GameObject>(newObject);
		return *gameObjects[newObject->GetId()].get();
	}
	GameObject& BtnScene::CreateNewGameObject(std::string objectName) {
		GameObject* newObject = gameReg.CreateNewGameObject();
		for (auto& [tag] : gameReg.GetAllOf<TagComponenet>()) {
			if (tag.tag == objectName) {
				objectName.append(std::to_string(newObject->GetId()));
			}
		}

		newObject->AddComponent<TagComponenet>(objectName);
		newObject->AddComponent<TransformComponent>();

		gameObjects[newObject->GetId()] = std::shared_ptr<GameObject>(newObject);
		return *gameObjects[newObject->GetId()].get();
	}

	GameObject& BtnScene::CreateNewGameObject(std::string objectName, uint64_t gameObjId) {
		GameObject* newObject = gameReg.CreateNewGameObject(gameObjId);
		for (auto& [tag] : gameReg.GetAllOf<TagComponenet>()) {
			if (tag.tag == objectName) {
				objectName.append(std::to_string(newObject->GetId()));
			}
		}

		newObject->AddComponent<TagComponenet>(objectName);
		newObject->AddComponent<TransformComponent>();

		gameObjects[newObject->GetId()] = std::shared_ptr<GameObject>(newObject);

		return *gameObjects[newObject->GetId()];
	}

	GameObject BtnScene::GetActiveCamera() {
		return GameObject((entt::entity)activeCameraId, this);
	}

	void BtnScene::ClearGameObjects() {
		for (auto& [gId, gObj] : gameObjects) {
			gObj->DestroyGameObject();
		}
		gameObjects.clear();
	}

	template<typename... Components>
	void CloneGameObject(entt::registry& r, entt::entity src, entt::entity dst) {
		([&] {
			if (auto* comp = r.try_get<Components>(src)) {
				r.emplace<Components>(dst, *comp);
			}
		 }(), ...);
	}

	//Duplicates game objects but doesn't copy some components like scriptig, physics, or collider components use a super gameobject for those
	GameObject& BtnScene::DuplicateGameObject(GameObject* gameObj) {
		GameObject* newObject = gameReg.CreateNewGameObject();
		std::string objectName = gameObj->GetComponent<TagComponenet>().tag;
		for (auto& [tag] : gameReg.GetAllOf<TagComponenet>()) {
			if (tag.tag == objectName) {
				objectName.append(std::to_string(newObject->GetId()));
			}
		}
		newObject->AddComponent<TagComponenet>().tag = objectName;
		CloneGameObject<TransformComponent, ModelComponent, LightComponent, CameraComponent, ArmatureComponent, AnimatorComponent, BoneComponent, AudioSourceComponent>(gameReg.GetNative(), static_cast<entt::entity>(gameObj->GetId()), static_cast<entt::entity>(newObject->GetId()));

		gameObjects[newObject->GetId()] = std::shared_ptr<GameObject>(newObject);
		return *gameObjects[newObject->GetId()].get();
	}

	void BtnScene::DeleteGameObject(GameObject* gameObject) {
		if (!gameObject->IsValid()) {
			return;
		}
		if (gameObject->CheckObjectForComponent<PhysicsComponet>()) {
			physics->RemoveObject(*gameObject);
		}
		if (gameObject->CheckObjectForComponent<AudioSourceComponent>()) {
			gameObject->GetComponent<AudioSourceComponent>().source.DestroySource();
		}
		if (gameObject->GetChildren().size() > 0) {
			unsigned int nChildren = gameObject->GetChildren().size();
			for (unsigned int i = 0; i < nChildren; i++) {
				GameObject child = gameObject->GetChildren()[i];
				DeleteGameObject(&child);
			}
		}

		if (gameObject->IsChild()) {
			gameObject->GetParent()->RemoveChild(*gameObject);
		}

		unsigned int gId = gameObject->GetId();
		gameReg.GetNative().destroy(static_cast<entt::entity>(gId));
		gameObjects.erase(gId);
	}

	glm::quat ExtractRotation(glm::mat4& m, GameObject gameObj) {
		glm::vec3 scale = gameObj.GetComponent<TransformComponent>().transform.scale;
		glm::mat3 rotMat;
		rotMat[0] = glm::vec3(m[0]) / scale.x;
		rotMat[1] = glm::vec3(m[1]) / scale.y;
		rotMat[2] = glm::vec3(m[2]) / scale.z;
		return glm::normalize(glm::quat_cast(rotMat));
	}

	void BtnScene::OnRuntimeCreate() {
		for (auto& [scriptComp] : gameReg.GetAllOf<ScriptComponent>()) {
			for (auto& scriptEntry : scriptComp.scripts) {
				BtnScript* script = scriptEntry.script;
				if (script && scriptEntry.isEnabled) {
					script->OnCreate();
				}
			}
		}
	}

	void BtnScene::OnRuntimeStart() {
		for (auto& [scriptComp] : gameReg.GetAllOf<ScriptComponent>()) {
			for (auto& scriptEntry : scriptComp.scripts) {
				BtnScript* script = scriptEntry.script;
				if (script && scriptEntry.isEnabled) {
					script->OnStart();
				}
			}
		}
		for (auto& [animComp] : gameReg.GetAllOfRef<AnimatorComponent>()) {
			animComp.get().SetState("DEFAULT", true);
			animComp.get().currentAnimation.isPlaying = true;
		}
	}
	void BtnScene::OnRuntimeUpdate() {

		for (auto& [scriptComp] : gameReg.GetAllOf<ScriptComponent>()) {
			for (auto& scriptEntry : scriptComp.scripts) {
				BtnScript* script = scriptEntry.script;
				if (scriptEntry.isEnabled) {
					script->OnUpdate(BtnTime::GetDeltaTime());
				}
			}
		}
		for (auto& [boneComp] : gameReg.GetAllOf<BoneComponent>()) {
			if (boneComp.selectedBone && !boneComp.selectedBone->name.empty()) {
				GameObject& gObj = *gameObjects[boneComp.targetId];
				auto& transform = gObj.GetComponent<TransformComponent>();

				glm::mat4 armatureWorld = gameObjects[boneComp.armatureId]->GetComponent<TransformComponent>().transform.GetMatrix();
				glm::mat4 boneWorld = armatureWorld * boneComp.selectedBone->globalTransform;
				glm::vec3 bonePos = glm::vec3(boneWorld[3]);
				glm::quat boneRot = glm::quat_cast(boneWorld);

				if (boneComp.useParentArmature) {
					if (boneComp.copyPos) {
						transform.transform.localPosition = bonePos - gameObjects[boneComp.armatureId]->GetComponent<TransformComponent>().transform.GetPosition();
					}
					if (boneComp.copyRot) {
						transform.transform.RotateLocalTo(boneRot);
					}
					continue;
				}
				transform.transform.MoveTo(bonePos);
				transform.transform.RotateTo(boneRot);
			}
		}
	}
	void BtnScene::OnRuntimePhysicsUpdate() {
		for (auto& [scriptComp] : gameReg.GetAllOf<ScriptComponent>()) {
			for (auto& scriptEntry : scriptComp.scripts) {
				BtnScript* script = scriptEntry.script;
				if (scriptEntry.isEnabled) {
					script->OnPhyUpdate(BtnTime::GetDeltaTime());
				}
			}
		}
	}
	void BtnScene::OnRuntimeEnd() {
		for (auto& [scriptComp] : gameReg.GetAllOf<ScriptComponent>()) {
			for (auto& scriptEntry : scriptComp.scripts) {
				BtnScript* script = scriptEntry.script;
				if (scriptEntry.isEnabled) {
					script->OnDestroy();
				}
			}
		}
	}

	const std::vector<std::shared_ptr<BtnWidget>> BtnScene::GetWidgets() {
		std::vector<std::shared_ptr<BtnWidget>> Widgets;

		for (const auto& [WidgetComp]:gameReg.GetAllOf<WidgetCanvasComponent>()) {
			for (const auto& Widget:WidgetComp.Widgets) {
				Widgets.push_back(Widget);
			}
		}

		return Widgets;
	}

	void BtnScene::SetActiveCamera(uint64_t cameraId) {
		if (gameObjects[(unsigned int)cameraId]->IsValid()) {
			if (gameObjects[(unsigned int)cameraId]->CheckObjectForComponent<CameraComponent>()) {
				activeCameraId = cameraId;
				gameObjects[(unsigned int)activeCameraId]->GetComponent<CameraComponent>().isMainCamera = true;
			}
			for (auto& [id, cam] : gameReg.GetAllOfID<CameraComponent>()) {
				if (id != activeCameraId) {
					cam.isMainCamera = false;
				}
			}
		}
	}
	void BtnScene::SetPhysics(BtnPhysics* pInstance) {
		physics = pInstance;
	}
}
