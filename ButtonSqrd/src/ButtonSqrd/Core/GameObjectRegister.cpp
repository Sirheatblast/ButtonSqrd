#include"GameObjectRegister.h"
#include"GameObject.h"
#include"Scene.h"

#include"ButtonSqrd/Gui/Widgets/BtnButton.h"
#include"ButtonSqrd/Gui/Widgets/BtnImage.h"
#include"ButtonSqrd/Gui/Widgets/BtnTextBox.h"
#include"ButtonSqrd/Gui/Widgets/BtnSlider.h"

namespace BtnSqd {
	GameObjectRegister& GameObjectRegister::operator=(const GameObjectRegister& reg) {
		if (this != &reg) {
			gameReg.clear();

			std::unordered_map<entt::entity, GameObject> entityMap;

			for (auto entity = reg.gameReg.storage<entt::entity>()->begin(); entity != reg.gameReg.storage<entt::entity>()->end(); entity++) {
				auto srcEntity = *entity;
				if (reg.gameReg.valid(srcEntity)) {
					BtnUUID id = reg.gameReg.get<IDComponent>(srcEntity).uuid;
					std::string name = reg.gameReg.get<TagComponenet>(srcEntity).tag;
					entityMap[srcEntity] = scene->CreateNewGameObject(name,id);
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
				if (reg.gameReg.any_of<ArmatureComponent>(srcEntity)) {
					ArmatureComponent armature = reg.gameReg.get<ArmatureComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<ArmatureComponent>();
					entityMap[srcEntity].GetComponent<ArmatureComponent>() = armature;
				}
				if (reg.gameReg.any_of<AnimatorComponent>(srcEntity)) {
					AnimatorComponent anim = reg.gameReg.get<AnimatorComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<AnimatorComponent>();
					entityMap[srcEntity].GetComponent<AnimatorComponent>() = anim;
				}
				if (reg.gameReg.any_of<BoneComponent>(srcEntity)) {
					BoneComponent bone = reg.gameReg.get<BoneComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<BoneComponent>();
					BoneComponent& nBone = entityMap[srcEntity].GetComponent<BoneComponent>() = bone;
					if (scene->GetgameObjects()[bone.armatureId]->CheckObjectForComponent<ArmatureComponent>()) {
						auto& armature = scene->GetgameObjects()[bone.armatureId]->GetComponent<ArmatureComponent>().armature;
						nBone.selectedBone = &armature.GetBones()[armature.GetBoneIds().at(bone.boneName)];
					}
				}
				if (reg.gameReg.any_of<AudioListenerComponent>(srcEntity)) {
					AudioListenerComponent listener = reg.gameReg.get<AudioListenerComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<AudioListenerComponent>();
					entityMap[srcEntity].GetComponent<AudioListenerComponent>() = listener;
				}
				if (reg.gameReg.any_of<AudioSourceComponent>(srcEntity)) {
					AudioSourceComponent aSource = reg.gameReg.get<AudioSourceComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<AudioSourceComponent>();
					entityMap[srcEntity].GetComponent<AudioSourceComponent>() = aSource;
				}
				if (reg.gameReg.any_of<ParticleEmitterComponent>(srcEntity)) {
					ParticleEmitterComponent particEmitter = reg.gameReg.get<ParticleEmitterComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<ParticleEmitterComponent>();
					entityMap[srcEntity].GetComponent<ParticleEmitterComponent>() = particEmitter;
				}
				if (reg.gameReg.any_of<WidgetCanvasComponent>(srcEntity)) {
					WidgetCanvasComponent wCanvas = reg.gameReg.get<WidgetCanvasComponent>(srcEntity);
					entityMap[srcEntity].AddComponent<WidgetCanvasComponent>();
					auto& wComp = entityMap[srcEntity].GetComponent<WidgetCanvasComponent>();
					for (const auto widget : wCanvas.Widgets) {
						switch (widget->GetType()) {
						case BtnWidgetType::Button:
							wComp.Widgets.push_back(std::make_shared<BtnButton>(*std::dynamic_pointer_cast<BtnButton>(widget)));
							break;
						case BtnWidgetType::Slider:
							wComp.Widgets.push_back(std::make_shared<BtnSlider>(*std::dynamic_pointer_cast<BtnSlider>(widget)));
							break;
						case BtnWidgetType::Text:
							wComp.Widgets.push_back(std::make_shared<BtnTextBox>(*std::dynamic_pointer_cast<BtnTextBox>(widget)));
							break;
						case BtnWidgetType::BtnImage:
							wComp.Widgets.push_back(std::make_shared<BtnImage>(*std::dynamic_pointer_cast<BtnImage>(widget)));
							break;
						}
					}
					wComp.dimensions = wCanvas.dimensions;
					wComp.posOffset = wCanvas.posOffset;
					wComp.useWholeScreen = wCanvas.useWholeScreen;
					wComp.displayInEditor = wCanvas.displayInEditor;
					wComp.selectedWidget = nullptr;
				}
			}
		}
		return *this;
	}
	GameObject* GameObjectRegister::CreateNewGameObject(){
		GameObject* newGameObject = new GameObject(gameReg.create(), scene);
		return newGameObject;
	}

	void GameObjectRegister::SetScene(BtnScene* currentScene) {
		scene = currentScene;
	}
}