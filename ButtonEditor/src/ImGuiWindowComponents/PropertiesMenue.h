#pragma once
#include<ButtonSqrd.h>
#include"ButtonSqrd/Core/GameObject.h"
#include"ButtonSqrd/Core/CoreComponents.h"
#include"ButtonSqrd/Core/ResourceManager.h"
#include"ButtonSqrd/Physics/BtnPhysics.h"
#include"ButtonSqrd/Event/CameraEvent.h"
#include"ButtonSqrd/Event/GuiEvent.h"
#include"../EditorEvents.h"
#include"BtnCurve.h"

#include"ButtonSqrd/Gui/Widgets/BtnTextBox.h"

#include<string>
#include<imgui.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<type_traits>
#include<filesystem>

namespace BtnSqd {
	class PropertiesMenue {
	public:
		PropertiesMenue(std::shared_ptr<BtnScene>& scene, BtnPhysics& physics);
		void OnUpdate(GameObject& selectedObj);
		void AddComponents(BtnSqd::GameObject& selectedObj);
		void SendEvent(Event* e);
	private:
		struct ParticleCurveHolder {
			ParticleCurveHolder() {}
			ParticleCurveHolder(unsigned int idx) :velCurve(BtnCurve("Velocity Curve", idx, 3, ImVec2(0.0f, -100.0f), ImVec2(150.0f, 100.0f), ImColor(1.0f, 0.0f, 0.0f, 1.0f))), fadeCurve(BtnCurve("Fade Curve", idx)), colorCurve(BtnCurve("Color Curve", idx)), sizeCurve(BtnCurve("Size Curve", idx)) {}

			BtnCurve fadeCurve;
			BtnCurve velCurve;
			BtnCurve colorCurve;
			BtnCurve sizeCurve;
		};


		void ShowVariableData(EditableData& data);

		void ShowColorPicker(glm::vec3& color, int index) {
			std::string tag = "##ColPickerLabel";
			tag += index;
			ImGui::ColorPicker3(tag.c_str(), glm::value_ptr(color));
		}
		void ShowColorPicker(glm::vec4& color, std::string tag) {
			ImGui::ColorPicker4(tag.c_str(), glm::value_ptr(color), ImGuiColorEditFlags_AlphaBar);
		}
		template<typename Component>
		bool ShowComponent(GameObject& selectedObj) {
			if (!currentScene->GetgameObjects()[selectedObj.GetId()]->CheckObjectForComponent<Component>()) {
				return false;
			}
			ImGui::Begin("Object properties");
			ImGui::Indent(20.0f);
			if (std::is_same<Component, TagComponenet>()) {
				TagComp(selectedObj);
			}
			else if (std::is_same<Component, TransformComponent>()) {
				TransformComp(selectedObj);
			}
			else if (std::is_same<Component, ModelComponent>()) {
				ModelComp(selectedObj);
			}
			else if (std::is_same<Component, LightComponent>()) {
				LightComp(selectedObj);
			}
			else if (std::is_same<Component, CameraComponent>()) {
				CameraComp(selectedObj);
			}
			else if (std::is_same<Component, ScriptComponent>()) {
				ScriptComp(selectedObj);
			}
			else if (std::is_same<Component, PhysicsComponet>()) {
				PhysicsComp(selectedObj);
			}
			else if (std::is_same<Component, ColliderComponent>()) {
				ColliderComp(selectedObj);
			}
			else if (std::is_same<Component, ArmatureComponent>()) {
				ArmatureComp(selectedObj);
			}
			else if (std::is_same<Component, AnimatorComponent>()) {
				AnimatorComp(selectedObj);
			}
			else if (std::is_same<Component, BoneComponent>()) {
				BoneComp(selectedObj);
			}
			else if (std::is_same<Component, AudioListenerComponent>()) {
				AudioListenerComp(selectedObj);
			}
			else if (std::is_same<Component, AudioSourceComponent>()) {
				AudioSourceComp(selectedObj);
			}
			else if (std::is_same<Component, ParticleEmitterComponent>()) {
				ParticleEmitterComp(selectedObj);
			}
			else if (std::is_same<Component, WidgetCanvasComponent>()) {
				WidgetCanvasComp(selectedObj);
			}
			ImGui::Unindent(20.0f);
			ImGui::End();

			return true;
		}

		void ModelComp(GameObject& selectedObj);
		void SetModel(std::string& fullPath, BtnSqd::ModelComponent& modelComp);
		void TransformComp(GameObject& selectedObj);
		void TagComp(GameObject& selectedObj);
		void LightComp(GameObject& selectedObj);
		void CameraComp(GameObject& selectedObj);
		void ScriptComp(GameObject& selectedObj);
		void PhysicsComp(GameObject& selectedObj);
		void ColliderComp(GameObject& selectedObj);
		void ArmatureComp(GameObject& selectedObj);
		void AnimatorComp(GameObject& selectedObj);
		void BoneComp(GameObject& selectedObj);
		void AudioListenerComp(GameObject& selectedObj);
		void AudioSourceComp(GameObject& selectedObj);
		void ParticleEmitterComp(GameObject& selectedObj);
		void WidgetCanvasComp(GameObject& selectedObj);

		void CreateWidgetPopup(WidgetCanvasComponent& wCanvas);
		
		void DrawParticleGUI(BtnSqd::ParticleEmitterComponent& partic, BtnSqd::GameObject& selectedObj);
		void RenderCurveConfig(unsigned int idx, BtnSqd::GameObject& selectedObj, bool show, bool& useCurve, std::vector<glm::vec2>& curvePoints, std::string curveLabel, BtnCurve& curve);
		void RenderMultiCurveConfig(unsigned int idx, BtnSqd::GameObject& selectedObj, bool show, bool& useCurve, std::vector<std::vector<glm::vec2>>& curvePoints, std::string curveLabel, BtnCurve& curve);
		void DrawCurve(BtnSqd::ParticleEmitterComponent& partic);
		void ProcessCurves(BtnSqd::ParticleEmitterComponent& partic);
		void CreateCollider(BtnSqd::GameObject& selectedObj, BtnSqd::ColliderComponent& collider);
		void AddScriptPopup(GameObject& selectedObject, ScriptComponent& scriptComp);

		void UpdateParticleCurves(GameObject& selectedObject);

		void TexturePicker(std::shared_ptr<Texture>& texture, std::vector<bool>::reference flag, glm::vec3 clearColor = glm::vec3(), int index = 0);

		void DrawBtnTextBoxData(std::shared_ptr<BtnWidget>widget);
		bool OnGetWidget(BtnSqd::OnGetWidgetEvent* e);

		bool showAddCompWindow = false;
		bool showAddScript = false;
		bool showAddModel = false;
		bool lightColorPicker = false;
		bool scaleUniform = false;
		bool addSound = false;
		bool showCurve = false;
		bool showGui = false;

		std::shared_ptr<BtnScene>& currentScene;
		std::vector<bool>showTexturePickerMain;
		std::vector<bool>showTexturePickerNormal;
		std::vector<bool>showColorPicker;

		std::vector<ParticleCurveHolder>showCurves;

		std::map<BtnWidgetType, std::function<void(std::shared_ptr<BtnWidget>)>> widgetTypeDrawCallbacks;

		BtnPhysics& btnPhysics;
		uint32_t selectedID = 0;
		BtnCurve* selectedCurve = nullptr;
	};
}

