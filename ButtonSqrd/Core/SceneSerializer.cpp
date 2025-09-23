#include "SceneSerializer.h"
#include"GameObject.h"
#include"SuperGameObject.h"
#include"ResourceManager.h"

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.a);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.a = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::mat3> {
		static Node encode(const glm::mat3& rhs) {
			Node node;
			node.push_back(rhs[0]);
			node.push_back(rhs[1]);
			node.push_back(rhs[2]);
			return node;
		}

		static bool decode(const Node& node, glm::mat3& rhs) {
			if (!node.IsSequence()) {
				return false;
			}

			rhs[0] = node[0].as<glm::vec3>();
			rhs[1] = node[1].as<glm::vec3>();
			rhs[2] = node[2].as<glm::vec3>();
			return true;
		}
	};

	template<>
	struct convert<glm::mat4> {
		static Node encode(const glm::mat4& rhs) {
			Node node;
			node.push_back(rhs[0]);
			node.push_back(rhs[1]);
			node.push_back(rhs[2]);
			node.push_back(rhs[3]);
			return node;
		}

		static bool decode(const Node& node, glm::mat4& rhs) {
			if (!node.IsSequence()) {
				return false;
			}

			rhs[0] = node[0].as<glm::vec4>();
			rhs[1] = node[1].as<glm::vec4>();
			rhs[2] = node[2].as<glm::vec4>();
			rhs[3] = node[3].as<glm::vec4>();
			return true;
		}
	};
}
namespace BtnSqd {

	static YAML::Emitter& operator<<(YAML::Emitter& out,glm::vec3 vec) {
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec4 vec) {
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z<<vec.a << YAML::EndSeq;
		return out;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, glm::mat3 mat) {
		out << YAML::Flow;
		out << YAML::BeginSeq << mat[0] << mat[1] << mat[2] << YAML::EndSeq;
		return out;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, glm::mat4 mat) {
		out << YAML::Flow;
		out << YAML::BeginSeq << mat[0] << mat[1] << mat[2]<<mat[3] << YAML::EndSeq;
		return out;
	}

	static void SerializeGameObject(YAML::Emitter& out, GameObject gameObject) {
		out << YAML::BeginMap;
		out << YAML::Key << "GameObject" << YAML::Value << gameObject.GetId();

		out << YAML::Key << "ChildrenIds" << YAML::BeginSeq;
		for (auto child:gameObject.GetChildren()) {
			out << YAML::Value << child.GetId();
		}
		out << YAML::EndSeq;

		if (gameObject.CheckObjectForComponent<TagComponenet>()) {
			TagComponenet& tag = gameObject.GetComponent<TagComponenet>();
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Tag" << YAML::Value << tag.tag;

			out << YAML::EndMap;
		}
		if (gameObject.CheckObjectForComponent<TransformComponent>()) {
			TransformComponent& transform = gameObject.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "rotation" << YAML::Value << transform.transform.rotation;
			out << YAML::Key << "front" << YAML::Value << transform.transform.front;
			out << YAML::Key << "up" << YAML::Value << transform.transform.up;
			out << YAML::Key << "scale" << YAML::Value << transform.transform.scale;

			if (gameObject.IsChild()) {
				out << YAML::Key << "localPosition" << YAML::Value << transform.transform.localPosition;
			}

			out << YAML::Key << "transformMat" << YAML::Value << transform.transform.GetTransformMat();
			out << YAML::Key << "rotationMat" << YAML::Value << transform.transform.GetRotationMat();

			out << YAML::EndMap;
		}
		if (gameObject.CheckObjectForComponent<ModelComponent>()) {
			ModelComponent& model = gameObject.GetComponent<ModelComponent>();
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap;
			
			out << YAML::Key << "directory" << YAML::Value << model.currentModel.GetDirectory();
			out << YAML::Key << "base shader" << YAML::Value << model.currentModel.GetShader()->GetShaderName();

			unsigned int meshCount = 0;
			out << YAML::Key << "Meshes" << YAML::BeginSeq;
			for (auto& mesh : *model.currentModel.GetMeshes()) {
				Material mat = *mesh.GetMaterial();
				out << YAML::BeginMap;
				out << YAML::Key << "MeshID" << YAML::Value << meshCount;
				out << YAML::Key << "ClearColor" << YAML::Value << mat.clearColor;
				if (mat.albedo) {
					out << YAML::Key << "AlbedoTex" << YAML::Value << mat.albedo->GetPath();
				}
				if (mat.normal) {
					out << YAML::Key << "NormalTex" << YAML::Value << mat.normal->GetPath();
				}
				out << YAML::Key << "Metallic" << YAML::Value << mat.metallic;
				out << YAML::Key << "Reflectancce" << YAML::Value << mat.reflectance;
				out << YAML::Key << "Roughness" << YAML::Value << mat.roughness;
				out << YAML::EndMap;
				meshCount++;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}
		if (gameObject.CheckObjectForComponent<LightComponent>()) {
			LightComponent& light = gameObject.GetComponent<LightComponent>();
			out << YAML::Key << "LightComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "lightColor" << light.lightColor;
			out << YAML::Key << "lightStrength" << light.strength;
			out << YAML::Key << "lightType" << light.lightType;
			out << YAML::Key << "lightOuterCutOff" << light.outerCutOff;
			out << YAML::Key << "lightCutOff" << light.cutOff;
			out << YAML::Key << "lightMat" << light.lightMat;

			out << YAML::EndMap;
		}
		if (gameObject.CheckObjectForComponent<CameraComponent>()) {
			CameraComponent& camera = gameObject.GetComponent<CameraComponent>();
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "viewMatrix" << YAML::Value << camera.viewMatrix;
			out << YAML::Key << "projectionMatrix" << YAML::Value << camera.projectionMatrix;
			out << YAML::Key << "isMainCamera" << YAML::Value << camera.isMainCamera;
			out << YAML::Key << "fov" << YAML::Value << camera.fov;
			out << YAML::Key << "aspectRatio" << YAML::Value << camera.aspectRatio;
			out << YAML::Key << "nearPlain" << YAML::Value << camera.nearPlain;
			out << YAML::Key << "farPlain" << YAML::Value << camera.farPlain;

			out << YAML::EndMap;
		}
		if (gameObject.CheckObjectForComponent<PhysicsComponet>()) {
			PhysicsComponet& physics = gameObject.GetComponent<PhysicsComponet>();

			out << YAML::Key << "PhysicsComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "RotatoinLockX" << YAML::Value << physics.lockX;
			out << YAML::Key << "RotatoinLockY" << YAML::Value << physics.lockY;
			out << YAML::Key << "RotatoinLockZ" << YAML::Value << physics.lockZ;

			out << YAML::Key << "isDynamic" << YAML::Value << physics.isDynamic;
			out << YAML::Key << "isKinematic" << YAML::Value << physics.isKinematic;
			out << YAML::Key << "mass" << YAML::Value << physics.mass;

			out << YAML::EndMap;
		}
		if (gameObject.CheckObjectForComponent<ColliderComponent>()) {
			ColliderComponent& collider = gameObject.GetComponent<ColliderComponent>();
	
			out << YAML::Key << "ColliderComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "staticFriction" << YAML::Value << collider.staticFriction;
			out << YAML::Key << "dynamicFriction" << YAML::Value << collider.dynamicFriction;
			out << YAML::Key << "resitution" << YAML::Value << collider.resitution;

			out << YAML::Key << "scale" << YAML::Value << collider.scale;
			out << YAML::Key << "sameAsTransform" << YAML::Value << collider.sameAsTransform;
			out << YAML::Key << "isSelectable" << YAML::Value << collider.isSelectalbe;
			out << YAML::Key << "isTrigger" << YAML::Value << collider.isTrigger;

			out << YAML::Key << "radius" << YAML::Value << collider.radius;
			out << YAML::Key << "halfHeight" << YAML::Value << collider.halfHeight;

			out << YAML::Key << "colliderType" << YAML::Value << collider.colliderType;
			out << YAML::EndMap;
		}
		if (gameObject.CheckObjectForComponent<ScriptComponent>()) {
			ScriptComponent& scripts = gameObject.GetComponent<ScriptComponent>();
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;

			out <<YAML::Key<<"Scripts" <<YAML::Value<< YAML::BeginSeq;
			for (auto& script:scripts.scripts) {
				out << YAML::BeginMap;
				out<<YAML::Key<<"name"<< YAML::Value << script.name;
				out<<YAML::Key<<"isEnabled"<< YAML::Value << script.isEnabled;

				uint32_t count=0;
				EditableData* scriptVars = script.script->GetEditables(count);
				out << YAML::Key << "EditableVars" << YAML::Value << YAML::BeginSeq;
				for (uint32_t i = 0; i < count;i++) {
					EditableData data = scriptVars[i];
					out << YAML::BeginMap;
					out << YAML::Key << "name" << YAML::Value << data.name;
					out << YAML::Key << "type" << YAML::Value << static_cast<int>(data.type);
					SerializeDataType(data, out);
					out << YAML::EndMap;
				}
					out << YAML::EndSeq;

				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}

	void SerializeDataType(EditableData& data, YAML::Emitter& out) {
		switch (data.type) {
		case DataType::Int: {
			out << YAML::Key << "value" << YAML::Value << *static_cast<int*>(data.data);
			break;
		}
		case DataType::Float: {
			out << YAML::Key << "value" << YAML::Value << *static_cast<float*>(data.data);
			break;
		}
		case DataType::Double: {
			out << YAML::Key << "value" << YAML::Value << *static_cast<double*>(data.data);
			break;
		}
		case DataType::Bool: {
			out << YAML::Key << "value" << YAML::Value << *static_cast<bool*>(data.data);
			break;
		}
		case DataType::String: {
			out << YAML::Key << "value" << YAML::Value << *static_cast<std::string*>(data.data);
			break;
		}
		case DataType::GameObject: {
			GameObject* gameObj = static_cast<GameObject*>(data.data);
			out << YAML::Key << "value" << YAML::Value << gameObj->GetId();
			break;
		}
		case DataType::SuperGameObject: {
			SuperGameObject* super = static_cast<SuperGameObject*>(data.data);
			out << YAML::Key << "value" << YAML::Value << super->GetPath();
		}
		}
	}

	void BtnSqd::SceneSerializer::SetChildren(YAML::Node& data) {
		auto gameObjects = data["GameObjects"];
		for (auto gameObject : gameObjects) {
			uint64_t gameObjId = gameObject["GameObject"].as<uint64_t>();
			auto childrenIds = gameObject["ChildrenIds"];
			for (auto child:childrenIds) {
				uint64_t childId = child.as <uint64_t>();
				scene->GetgameObjects()[gameObjId]->AddChild(*scene->GetgameObjects()[childId]);
			}
		}
	}

	void BtnSqd::SceneSerializer::Serialize(std::string path, std::string name) {
		if (name == "") {
			name = scene->GetName();
		}
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << name;
		out << YAML::Key << "SkyBoxTexture" << YAML::Value << scene->skyboxId;
		out << YAML::Key << "GameObjects" << YAML::Value << YAML::BeginSeq;
		for (entt::entity entity : scene->gameReg.GetNative().view<entt::entity>()) {
			GameObject gameObject = *scene->GetgameObjects()[static_cast<uint32_t>(entity)];
			if (!gameObject.IsValid())
				return;
			SerializeGameObject(out, gameObject);
		}

		out << YAML::EndMap;
		std::ofstream fout(path);
		fout << out.c_str();
	}

	void BtnSqd::SceneSerializer::SerializeRuntime(std::string path) {
		BTN_CORE_LOG_ERROR("DO NOT USE YET, WIP");
	}

	bool BtnSqd::SceneSerializer::Deserialize(std::string path) {
		std::ifstream sceneFile(path);
		std::stringstream iss;

		iss << sceneFile.rdbuf();
		YAML::Node data = YAML::Load(iss.str());
		if (!data["Scene"]) {
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		BTN_CORE_LOG_TRACE("Loading Scene: " + sceneName);

		std::string skyboxPath;
		if (data["SkyBoxTexture"]) {
			std::string skyboxPath = data["SkyBoxTexture"].as<std::string>();
			scene->SetSkyboxTexture(skyboxPath);
		}

		auto gameObjects = data["GameObjects"];
		if (gameObjects) {
			for (auto gameObject : gameObjects) {
				uint64_t gameObjId = gameObject["GameObject"].as<uint64_t>();
				std::string name;

				auto tagComp = gameObject["TagComponent"];
				if (tagComp) {
					name = tagComp["Tag"].as<std::string>();
				}
				GameObject& loadedGameObj = scene->CreateNewGameObject(name,gameObjId);

				auto transformComp = gameObject["TransformComponent"];
				if (transformComp) {
					glm::vec3 rotation = transformComp["rotation"].as<glm::vec3>();
					glm::vec3 front = transformComp["front"].as<glm::vec3>();
					glm::vec3 up = transformComp["up"].as<glm::vec3>();
					glm::vec3 scale = transformComp["scale"].as<glm::vec3>();

					TransformComponent& transform = loadedGameObj.GetComponent<TransformComponent>();
					transform.transform.rotation = rotation;
					transform.transform.front = front;
					transform.transform.up = up;
					transform.transform.scale = scale;

					if (transformComp["localPosition"]) {
						transform.transform.localPosition = transformComp["localPosition"].as<glm::vec3>();
					}

					transform.transform.transformMat = transformComp["transformMat"].as<glm::mat4>();
					transform.transform.rotationMat = transformComp["rotationMat"].as<glm::mat4>();
				}

				auto lightComp = gameObject["LightComponent"];
				if (lightComp) {
					loadedGameObj.AddComponent<LightComponent>();
					LightComponent& light = loadedGameObj.GetComponent<LightComponent>();

					light.lightColor = lightComp["lightColor"].as<glm::vec3>();
					light.strength = lightComp["lightStrength"].as<float>();
					light.lightType = (LightType)lightComp["lightType"].as<unsigned int>();
					light.outerCutOff = lightComp["lightOuterCutOff"].as<float>();
					light.cutOff = lightComp["lightCutOff"].as<float>();
					light.lightMat = lightComp["lightMat"].as<glm::mat4>();
				}

				auto modelComp = gameObject["ModelComponent"];
				if (modelComp) {
					loadedGameObj.AddComponent<ModelComponent>();
					ModelComponent& model = loadedGameObj.GetComponent<ModelComponent>();
					std::string directory = modelComp["directory"].as<std::string>();
					model.currentModel = ResourceManager::GetLoadedModels()[directory];
					auto meshes = modelComp["Meshes"];
					if (meshes.IsSequence()) {
						for(auto meshInfo:meshes) {
							unsigned int id = meshInfo["MeshID"].as<unsigned int>();
							auto& mesh = model.currentModel.GetMeshes()[0][id];
							Material& mat = *mesh.GetMaterial();

							if (meshInfo["AlbedoTex"]) {
								mat.albedo = ResourceManager::GetLoadedTextures()[meshInfo["AlbedoTex"].as<std::string>()];
							}
							if (meshInfo["NormalTex"]) {
								mat.normal = ResourceManager::GetLoadedTextures()[meshInfo["NormalTex"].as<std::string>()];
							}

							mat.clearColor = meshInfo["ClearColor"].as<glm::vec3>();
							mat.roughness = meshInfo["Roughness"].as<float>();
							mat.reflectance = meshInfo["Reflectancce"].as<float>();
							mat.metallic = meshInfo["Metallic"].as<float>();
						}
					}
				}

				auto cameraComp = gameObject["CameraComponent"];
				if (cameraComp) {
					loadedGameObj.AddComponent<CameraComponent>(loadedGameObj.GetComponent<TransformComponent>());
					CameraComponent& camera = loadedGameObj.GetComponent<CameraComponent>();

					camera.viewMatrix = cameraComp["viewMatrix"].as<glm::mat4>();
					camera.projectionMatrix = cameraComp["projectionMatrix"].as<glm::mat4>();

					camera.isMainCamera = cameraComp["isMainCamera"].as<bool>();
					if (camera.isMainCamera) {
						scene->SetActiveCamera(gameObjId);
					}
					camera.fov = cameraComp["fov"].as<float>();
					camera.aspectRatio = cameraComp["aspectRatio"].as<float>();
					camera.nearPlain = cameraComp["nearPlain"].as<float>();
					camera.farPlain = cameraComp["farPlain"].as<float>();
				}

				auto physicsComp = gameObject["PhysicsComponent"];
				if (physicsComp) {
					PhysicsComponet& physics = loadedGameObj.AddComponent<PhysicsComponet>();

					physics.lockX = physicsComp["RotatoinLockX"].as<bool>();
					physics.lockY = physicsComp["RotatoinLockY"].as<bool>();
					physics.lockZ = physicsComp["RotatoinLockZ"].as<bool>();

					physics.isDynamic = physicsComp["isDynamic"].as<bool>();
					physics.isKinematic = physicsComp["isKinematic"].as<bool>();
					physics.mass = physicsComp["mass"].as<float>();
				}

				auto colliderComp = gameObject["ColliderComponent"];
				if (colliderComp) {
					ColliderComponent& collider = loadedGameObj.AddComponent<ColliderComponent>();

					collider.staticFriction = colliderComp["staticFriction"].as<float>();
					collider.dynamicFriction = colliderComp["dynamicFriction"].as<float>();
					collider.resitution = colliderComp["resitution"].as<float>();
					collider.scale = colliderComp["scale"].as<glm::vec3>();
					collider.sameAsTransform = colliderComp["sameAsTransform"].as<bool>();
					collider.isSelectalbe = colliderComp["isSelectable"].as<bool>();
					collider.isTrigger = colliderComp["isTrigger"].as<bool>();
					collider.radius = colliderComp["radius"].as<float>();
					collider.halfHeight = colliderComp["halfHeight"].as<float>();
					collider.colliderType = static_cast<ColliderType>(colliderComp["colliderType"].as<int>());
				}

			}
		}

		LoadScriptComponent(gameObjects);
		SetChildren(data);
		scene->sceneName = sceneName;		


		BTN_CORE_LOG_TRACE("Loading Scene Complete: " + sceneName);
		return true;
	}

	void SceneSerializer::LoadScriptComponent(YAML::Node& gameObjects) {
		for (auto gameObject : gameObjects) {
			uint64_t gameObjId = gameObject["GameObject"].as<uint64_t>();
			std::string name;

			auto tagComp = gameObject["TagComponent"];
			if (tagComp) {
				name = tagComp["Tag"].as<std::string>();
			}
			GameObject& loadedGameObj = *scene->GetgameObjects()[gameObjId];

			auto scriptComp = gameObject["ScriptComponent"];
			if (scriptComp) {
				ScriptComponent& script = loadedGameObj.AddComponent < ScriptComponent>();
				for (auto scriptNode : scriptComp["Scripts"]) {
					std::string scriptName = scriptNode["name"].as<std::string>();
					script.AddScript(scene->gameObjects[loadedGameObj.GetId()].get(), "../ButtonEditor/EngineAssets/Scripts/bin/BtnScripts.dll", scriptName, scene);
					for (auto& spt : script.scripts) {
						if (spt.name == scriptName) {
							spt.isEnabled = scriptNode["isEnabled"].as<bool>();
							SetInternalVars(spt, scriptNode);
							break;
						}
					}
				}
			}
		}
	}

	void SceneSerializer::SetInternalVars(BtnSqd::ScriptData& spt, YAML::detail::iterator_value& scriptNode) {
		uint32_t count = 0;
		uint32_t i = 0;
		EditableData* vars = spt.script->GetEditables(count);
		for (auto dataNode : scriptNode["EditableVars"]) {
			EditableData data = vars[i];
			switch (data.type) {
			case DataType::Int: {
				*static_cast<int*>(data.data) = dataNode["value"].as<int>();
				break;
			}
			case DataType::Float: {
				*static_cast<float*>(data.data) = dataNode["value"].as<float>();
				break;
			}
			case DataType::Double: {
				*static_cast<double*>(data.data) = dataNode["value"].as<double>();
				break;
			}
			case DataType::Bool: {
				*static_cast<bool*>(data.data) = dataNode["value"].as<bool>();
				break;
			}
			case DataType::String: {
				*static_cast<std::string*>(data.data) = dataNode["value"].as<std::string>();
				break;
			}
			case DataType::GameObject: {
				*static_cast<GameObject*>(data.data) = *scene->GetgameObjects()[dataNode["value"].as<uint32_t>()];
				break;
			}
			case DataType::SuperGameObject: {
				*static_cast<SuperGameObject*>(data.data) = *ResourceManager::GetLoadedSuperGameObjects()[dataNode["value"].as<std::string>()].get();
			}
			}
			i++;
		}
	}

	bool BtnSqd::SceneSerializer::DeserializeRuntime(std::string path) {
		BTN_CORE_LOG_ERROR("DO NOT USE YET, WIP");
		return true;
	}
}
