#include "SuperGameObject.h"

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
					//TODO
					break;
				}
			}
		}

		static YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec3 vec) {
			out << YAML::Flow;
			out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
			return out;
		}

		static YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec4 vec) {
			out << YAML::Flow;
			out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.a << YAML::EndSeq;
			return out;
		}

		static YAML::Emitter& operator<<(YAML::Emitter& out, glm::mat3 mat) {
			out << YAML::Flow;
			out << YAML::BeginSeq << mat[0] << mat[1] << mat[2] << YAML::EndSeq;
			return out;
		}

		static YAML::Emitter& operator<<(YAML::Emitter& out, glm::mat4 mat) {
			out << YAML::Flow;
			out << YAML::BeginSeq << mat[0] << mat[1] << mat[2] << mat[3] << YAML::EndSeq;
			return out;
		}

		void SuperGameObject::SetUpSGobj(GameObject* gameObject) {
			tag = gameObject->GetComponent<TagComponenet>();
			scale = gameObject->GetComponent<TransformComponent>().transform.scale;
			if (gameObject->CheckObjectForComponent<ModelComponent>()) {
				flags.hasModel = true;
				model = gameObject->GetComponent<ModelComponent>();
			}

			if (gameObject->CheckObjectForComponent<LightComponent>()) {
				flags.hasLight = true;
				light = gameObject->GetComponent<LightComponent>();
			}

			if (gameObject->CheckObjectForComponent<CameraComponent>()) {
				flags.hasCamera = true;
				camera = gameObject->GetComponent<CameraComponent>();
			}

			if (gameObject->CheckObjectForComponent<ScriptComponent>()) {
				flags.hasScript = true;
				script = gameObject->GetComponent<ScriptComponent>();
			}

			if (gameObject->CheckObjectForComponent<PhysicsComponet>()) {
				flags.hasPhysics = true;
				physics = gameObject->GetComponent<PhysicsComponet>();
			}

			if (gameObject->CheckObjectForComponent<ColliderComponent>()) {
				flags.hasCollider = true;
				collider = gameObject->GetComponent<ColliderComponent>();
			}
		}

		void SuperGameObject::SaveSuperGameObject() {
			YAML::Emitter out;

			out << YAML::BeginMap;
			out << YAML::Key << "SuperGameObject" <<YAML::Value<<tag.tag;
			out << YAML::Key << "scale" << YAML::Value << scale;

			out << YAML::Key << "hasModel" << YAML::Value << flags.hasModel;
			out << YAML::Key << "hasLight" << YAML::Value << flags.hasLight;
			out << YAML::Key << "hasCamera" << YAML::Value << flags.hasCamera;
			out << YAML::Key << "hasScript" << YAML::Value << flags.hasScript;
			out << YAML::Key << "hasPhysics" << YAML::Value << flags.hasPhysics;
			out << YAML::Key << "hasCollider" << YAML::Value << flags.hasCollider;

			if (flags.hasModel) {
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
			if (flags.hasLight) {
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
			if (flags.hasCamera) {
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
			if (flags.hasPhysics) {
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
			if (flags.hasCollider) {
				out << YAML::Key << "ColliderComponent";
				out << YAML::BeginMap;

				out << YAML::Key << "staticFriction" << YAML::Value << collider.staticFriction;
				out << YAML::Key << "dynamicFriction" << YAML::Value << collider.dynamicFriction;
				out << YAML::Key << "resitution" << YAML::Value << collider.resitution;

				out << YAML::Key << "scale" << YAML::Value << collider.scale;
				out << YAML::Key << "sameAsTransform" << YAML::Value << collider.sameAsTransform;

				out << YAML::Key << "radius" << YAML::Value << collider.radius;
				out << YAML::Key << "halfHeight" << YAML::Value << collider.halfHeight;

				out << YAML::Key << "colliderType" << YAML::Value << collider.colliderType;
				out << YAML::EndMap;
			}
			if (flags.hasScript) {
				out << YAML::Key << "ScriptComponent";
				out << YAML::BeginMap;

				out << YAML::Key << "Scripts" << YAML::Value << YAML::BeginSeq;
				for (auto& script : script.scripts) {
					out << YAML::BeginMap;
					out << YAML::Key << "name" << YAML::Value << script.name;
					out << YAML::Key << "isEnabled" << YAML::Value << script.isEnabled;

					uint32_t count = 0;
					EditableData* scriptVars = script.script->GetEditables(count);
					out << YAML::Key << "EditableVars" << YAML::Value << YAML::BeginSeq;
					for (uint32_t i = 0; i < count; i++) {
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

			std::string fullPath = path + tag.tag + ".SgObj";
			std::ofstream fileOut(fullPath);
			fileOut << out.c_str();
		}
		std::shared_ptr<SuperGameObject> SuperGameObject::LoadSuperGameObject(std::string path)	{
			std::shared_ptr<SuperGameObject> nCopy;
			nCopy.reset(new SuperGameObject());
			nCopy->path = path;

			std::ifstream inFile(path);
			std::stringstream iss;

			iss << inFile.rdbuf();
			YAML::Node data = YAML::Load(iss.str());
			if (!data["SuperGameObject"]) {
				return nullptr;
			}

			nCopy->tag.tag = data["SuperGameObject"].as<std::string>();
			nCopy->scale = data["scale"].as<glm::vec3>();

			nCopy->flags.hasModel = data["hasModel"].as<bool>();
			if (nCopy->flags.hasModel) {
				auto modelComp = data["ModelComponent"];

				ModelComponent& model = nCopy->model;
				std::string directory = modelComp["directory"].as<std::string>();
				model.currentModel = ResourceManager::GetLoadedModels()[directory];
				auto meshes = modelComp["Meshes"];
				if (meshes.IsSequence()) {
					for (auto meshInfo : meshes) {
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
			nCopy->flags.hasLight = data["hasLight"].as<bool>();
			if (nCopy->flags.hasLight) {
				auto lightComp = data["LightComponent"];
				LightComponent& light = nCopy->light;

				light.lightColor = lightComp["lightColor"].as<glm::vec3>();
				light.strength = lightComp["lightStrength"].as<float>();
				light.lightType = (LightType)lightComp["lightType"].as<unsigned int>();
				light.outerCutOff = lightComp["lightOuterCutOff"].as<float>();
				light.cutOff = lightComp["lightCutOff"].as<float>();
				light.lightMat = lightComp["lightMat"].as<glm::mat4>();
			}
			nCopy->flags.hasCamera = data["hasCamera"].as<bool>();
			if (nCopy->flags.hasCamera) {
				auto cameraComp = data["CameraComponent"];
				CameraComponent& camera = nCopy->camera;

				camera.viewMatrix = cameraComp["viewMatrix"].as<glm::mat4>();
				camera.projectionMatrix = cameraComp["projectionMatrix"].as<glm::mat4>();

				camera.isMainCamera = cameraComp["isMainCamera"].as<bool>();

				camera.fov = cameraComp["fov"].as<float>();
				camera.aspectRatio = cameraComp["aspectRatio"].as<float>();
				camera.nearPlain = cameraComp["nearPlain"].as<float>();
				camera.farPlain = cameraComp["farPlain"].as<float>();
			}
			nCopy->flags.hasPhysics = data["hasPhysics"].as<bool>();
			if (nCopy->flags.hasPhysics) {
				auto physicsComp = data["PhysicsComponent"];
				PhysicsComponet& physics = nCopy->physics;

				physics.lockX = physicsComp["RotatoinLockX"].as<bool>();
				physics.lockY = physicsComp["RotatoinLockY"].as<bool>();
				physics.lockZ = physicsComp["RotatoinLockZ"].as<bool>();

				physics.isDynamic = physicsComp["isDynamic"].as<bool>();
				physics.isKinematic = physicsComp["isKinematic"].as<bool>();
				physics.mass = physicsComp["mass"].as<float>();
			}
			nCopy->flags.hasCollider = data["hasCollider"].as<bool>();
			if (nCopy->flags.hasCollider) {
				auto colliderComp = data["ColliderComponent"];
				ColliderComponent& collider = nCopy->collider;

				collider.staticFriction = colliderComp["staticFriction"].as<float>();
				collider.dynamicFriction = colliderComp["dynamicFriction"].as<float>();
				collider.resitution = colliderComp["resitution"].as<float>();
				collider.scale = colliderComp["scale"].as<glm::vec3>();
				collider.sameAsTransform = colliderComp["sameAsTransform"].as<bool>();
				collider.radius = colliderComp["radius"].as<float>();
				collider.halfHeight = colliderComp["halfHeight"].as<float>();
				collider.colliderType = static_cast<ColliderType>(colliderComp["colliderType"].as<int>());
			}
			nCopy->flags.hasScript = data["hasScript"].as<bool>();
			if (nCopy->flags.hasScript) {
				auto scriptComp = data["ScriptComponent"];
				ScriptComponent& script = nCopy->script;
				for (auto scriptNode : scriptComp["Scripts"]) {
					std::string scriptName = scriptNode["name"].as<std::string>();
					script.AddScript(nCopy.get(), "./EngineAssets/Scripts/bin/BtnScripts.dll", scriptName);
					for (auto& spt : script.scripts) {
						if (spt.name == scriptName) {
							spt.isEnabled = scriptNode["isEnabled"].as<bool>();
							nCopy->InitVars(spt, scriptNode);
							break;
						}
					}
				}
			}

			return nCopy;
		}
		void SuperGameObject::InitVars(BtnSqd::ScriptData& spt, YAML::detail::iterator_value& scriptNode) {
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
						//TODO
						break;
					}
				}
				i++;
			}
		}
	}