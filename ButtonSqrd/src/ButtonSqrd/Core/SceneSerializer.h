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

	static void SerializeWidgetCanvasComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeParticleComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeAudioComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeAudioListenerComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeBoneComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeAnimatorComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeArmatureComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeScriptComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeColliderComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializePhysicsComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeCameraComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeLightComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeModelComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeTransformComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
	static void SerializeTagComp(BtnSqd::GameObject& gameObject, YAML::Emitter& out);
}