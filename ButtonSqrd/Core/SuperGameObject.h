#pragma once
#include"GameObject.h"
#include"CoreComponents.h"

#include<vector>
#include<string>
#include<yaml-cpp/yaml.h>
#include<fstream>

namespace BtnSqd {
	struct SuperFlags {
		bool hasModel = false;
		bool hasLight = false;
		bool hasCamera = false;
		bool hasScript = false;
		bool hasPhysics = false;
		bool hasCollider = false;
	};

	class SuperGameObject {
	public:
		SuperGameObject(GameObject* gameObject,std::string path) :path(path){
			SetUpSGobj(gameObject);
			SaveSuperGameObject();
		}
		SuperGameObject(){

		}

		void SaveSuperGameObject();
		static std::shared_ptr<SuperGameObject> LoadSuperGameObject(std::string path);
		bool IsValid() const { return !path.empty(); }

		glm::vec3 GetScale() { return scale; }
		TagComponenet GetTag() { return tag; }
		ModelComponent GetModel() { return model; }
		LightComponent GetLight() { return light; }
		CameraComponent GetCamera() { return camera; }
		ScriptComponent GetScript() { return script; }
		PhysicsComponet GetPhysics() { return physics; }
		ColliderComponent GetCollider() { return collider; }

		SuperFlags GetFlags() { return flags; }
		std::string GetPath(){ return path; }
	private:
		void SetUpSGobj(GameObject* gameObject);
		void InitVars(BtnSqd::ScriptData& spt, YAML::detail::iterator_value& scriptNode);

		glm::vec3 scale = glm::vec3(1.0f);

		TagComponenet tag;
		ModelComponent model;
		LightComponent light;
		CameraComponent camera;
		ScriptComponent script;
		PhysicsComponet physics;
		ColliderComponent collider;

		SuperFlags flags;
		std::string path;
	};
}
