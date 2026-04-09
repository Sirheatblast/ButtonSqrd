#pragma once

#include<ButtonSqrd.h>
#include<imgui.h>
#include<filesystem>

#include"ButtonSqrd/Core/SceneSerializer.h"
#include"../EditorEvents.h"
#include"ButtonSqrd/Event/SceneEvent.h"
#include"ButtonSqrd/Event/GuiEvent.h"

namespace BtnSqd {
	class SceneDirectory {
	public:
		SceneDirectory(std::shared_ptr<BtnScene>& currentScene);
		void OnUpdate(GameObject& selectedObj);
		void DropAddChild(std::shared_ptr<BtnSqd::GameObject>& gameObj);
		void DragGameObj(std::shared_ptr<BtnSqd::GameObject>& gameObj);
	private:
		void DestroySelected(GameObject& parent);
		void DisplayChildren(GameObject gameObj, GameObject& selectedObj);
		void SaveScene();
		void LoadScene();
		void SceneConfigMenue();
		void ChangeSkybox();

		std::shared_ptr<BtnScene>& currentScene;
		bool sceneConfigMenue = false;
		bool changeSkybox = false;
		bool createGameObj = false;
		bool saveScene = false;
		bool loadScene = false;
		char buffer[35] = "";
		std::string destination = "./Assets/Scenes/";
	};
}

