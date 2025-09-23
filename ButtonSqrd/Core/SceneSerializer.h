#pragma once
#include"Scene.h"
#include"GameObject.h"
#include"ResourceManager.h"

#include<yaml-cpp/yaml.h>
#include<string>
#include<fstream>

namespace BtnSqd {
	class SceneSerializer {
	public:
		SceneSerializer(BtnScene* currentScene):scene(currentScene){}

		void Serialize(std::string path="", std::string name = "");
		void SerializeRuntime(std::string path);

		bool Deserialize(std::string path);
		void SetInternalVars(BtnSqd::ScriptData& spt, YAML::detail::iterator_value& scriptNode);
		bool DeserializeRuntime(std::string path);

	private:
		void SetChildren(YAML::Node& data);
		void LoadScriptComponent(YAML::Node& gameObjects);

		BtnScene* scene;
	};
	static void SerializeDataType(EditableData& data, YAML::Emitter& out);
}