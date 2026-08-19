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
	private:
		void DropAddChild(std::shared_ptr<BtnSqd::GameObject> gameObj);
		void DragGameObj(GameObject* gameObj);
		void DragWidgetSource(std::shared_ptr<BtnWidget>& widget, std::shared_ptr<BtnSqd::GameObject>& gameObj);
		void DropWidgetTarget(std::shared_ptr<BtnWidget> nParent, std::shared_ptr<BtnSqd::GameObject>& gameObj);

		void DestroySelected(GameObject& parent);
		void DisplayGameObject(std::shared_ptr<BtnSqd::GameObject>& gameObj, GameObject& selectedObj);
		void HandleDisplayWidgets(std::shared_ptr<BtnSqd::GameObject>& gameObj, BtnSqd::GameObject& selectedObj);
		void DisplayWidgets(WidgetCanvasComponent& widgetComp, std::shared_ptr<BtnSqd::GameObject>& gameObj);
		void DisplayWidget(BtnWidget* widget, BtnSqd::WidgetCanvasComponent& widgetComp, std::shared_ptr<BtnSqd::GameObject>& gameObj, unsigned int i);
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

		std::unique_ptr<Texture> hiddenChildrenTexture;
		std::unique_ptr<Texture> showChildrenTexture;

		float childrenTexSize = 20.0f;
	};
}

