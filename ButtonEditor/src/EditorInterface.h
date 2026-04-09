#pragma once
#include<ButtonSqrd.h>
#include<imgui.h>
#include<ImGuizmo.h>
#include<filesystem>
#include<thread>
#include<atomic>

#include"ButtonSqrd/Event/CameraEvent.h"
#include"ButtonSqrd/Event/SceneEvent.h"
#include"ButtonSqrd/Physics/BtnPhysics.h"
#include"ButtonSqrd/Audio/AudioDevice.h"

#include"ImGuiWindowComponents/AssetDirectory.h"
#include"ImGuiWindowComponents/SceneDirectory.h"
#include"ImGuiWindowComponents/PropertiesMenue.h"
#include"ImGuiWindowComponents/EditorWindow.h"
#include"ImGuiWindowComponents/GameWindow.h"
#include"ImGuiWindowComponents/BtnConsole.h"
#include"ImGuiWindowComponents/AnimatorController.h"

#include"Scripting/ScriptManager.h"

namespace BtnSqd {
	class EditorInterface:public BtnSqd::Layer {
	public:
		EditorInterface();

		void  OnUpdate() override;
		void  OnAttach() override;
		void  OnDetach() override;
		void  OnEvent(BtnSqd::Event* e) override;

	private:
		void SetGuiStyle();
		void EditorInput();
		void CreateNewGameObjectWindow();

		bool OnMouseMoveEvent(BtnSqd::OnMouseMoveEvent* e);
		bool OnMouseClickEvent(BtnSqd::OnMouseClickEvent* e);
		bool OnMouseClickReleaseEvent(BtnSqd::OnMouseClickReleaseEvent* e);
		bool OnMouseScrollEvent(BtnSqd::OnMouseScrollEvent* e);
		bool OnKeyDownEvent(BtnSqd::OnKeyDownEvent* e);
		bool OnKeyReleaseEvent(BtnSqd::OnKeyReleaseEvent* e);
		bool OnWindowResizeEvent(BtnSqd::OnWindowResizeEvent* e);
		bool OnKeyTypedEvent(BtnSqd::OnKeyTypedEvent* e);
		bool OnSelectGameObject(OnSelectGameObjectEvent* e);
		bool OnSelectWindow(OnSelectWindowEvent* e);
		bool OnCreateGameObject(OnCreateGameObjectEvent* e);
		bool OnDestroyGameObject(OnDesroyGameObjectEvent* e);
		bool OnSelectCamera(OnSetActiveCameraEvent* e);
		bool OnChangeScene(OnChangeSceneEvent* e);
		bool OnStartRuntime(OnStartRuntimeEvent* e);
		bool OnPauseRuntime(OnPauseRuntimeEvent* e);
		bool OnEndRuntime(OnEndRuntimeEvent* e);

		void CompileScripts();

		ImGuiKey ConvertToImguiKey(KeyCode keycode);

		std::shared_ptr<BtnScene> currentScene;
		std::shared_ptr<BtnScene> editorScene;

		GameObject selectedObject;
		ActiveWindow selectedWindow;
		RuntimeState runtimeState = RuntimeState::End;

		bool showGameObjectCreateWindow = false;
		bool saveScenePossible = true;
		std::atomic<bool> isCompiled{ false };

		std::string destination = "./Assets/Scenes/";

		AssetDirectory assetDir;
		SceneDirectory sceneDir;
		PropertiesMenue propMenue;
		EditorWindow editorWindow;
		GameWindow gameWindow;
		BtnConsole console;
		AnimatorController animController;
		BtnPhysics btnPhysicObj;
		AudioDevice audioDev;
	};
}

