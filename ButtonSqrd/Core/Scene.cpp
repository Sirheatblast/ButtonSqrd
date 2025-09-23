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
        return *this;
    }

    GameObject& BtnScene::CreateNewGameObject() {
        GameObject* newObject = gameReg.CreateNewGameObject();
        std::string defName = "GameObject";
        defName+= std::to_string(newObject->GetId());
        newObject->AddComponent<TagComponenet>(defName);
        newObject->AddComponent<TransformComponent>();
        gameObjects[newObject->GetId()] = std::shared_ptr<GameObject>(newObject);
        return *gameObjects[newObject->GetId()].get();
    }
    GameObject& BtnScene::CreateNewGameObject(SuperGameObject* gTemplate) {
        GameObject* newObject = gameReg.CreateNewGameObject();
        newObject->AddComponent<TagComponenet>(gTemplate->GetTag().tag+std::to_string(newObject->GetId()));
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
            for (auto scriptData:gTemplate->GetScript().scripts) {
                script.AddScript(newObject, dllPath, scriptData.name,this);
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

        gameObjects[newObject->GetId()]=std::shared_ptr<GameObject>(newObject);
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

    void BtnScene::ClearGameObjects(){
        for (auto& [gId,gObj]:gameObjects) {
            gObj->DestroyGameObject();
        }
        gameObjects.clear();
    }
    void BtnScene::DeleteGameObject(GameObject* gameObject) {
        if (!gameObject->IsValid()) {
            return;
        }
        if (gameObject->CheckObjectForComponent<PhysicsComponet>()) {
            physics->RemoveObject(*gameObject);
        }

        if (gameObject->GetChildren().size()>0) {
            unsigned int nChildren = gameObject->GetChildren().size();
            for (unsigned int i = 0; i < nChildren;i++) {
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
    }
    void BtnScene::OnRuntimeUpdate() {

        for (auto& [scriptComp]: gameReg.GetAllOf<ScriptComponent>()) {
            for (auto& scriptEntry : scriptComp.scripts) {
                BtnScript* script = scriptEntry.script;
                if (scriptEntry.isEnabled) {
                    script->OnUpdate(BtnTime::GetDeltaTime());
                }
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
    void BtnScene::SetActiveCamera(uint64_t cameraId) {
        if (gameObjects[(unsigned int)activeCameraId]->IsValid() && gameObjects[(unsigned int)activeCameraId]->CheckObjectForComponent<CameraComponent>()) {
            gameObjects[(unsigned int)activeCameraId]->GetComponent<CameraComponent>().isMainCamera = false;
        }
        activeCameraId = cameraId;
    }
    void BtnScene::SetPhysics(BtnPhysics* pInstance) {
        physics = pInstance;
    }
}
