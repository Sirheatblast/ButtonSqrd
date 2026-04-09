#pragma once
#include"ButtonSqrd/Event/Event.h"
#include"ButtonSqrd/Core/GameObject.h"

#include"EditorFlags.h"

namespace BtnSqd {

	enum AssetRawType {
		ModelRaw=0,TextureRaw,ShaderRaw, GameObjectRaw,SuperGameObjectRaw
	};

	//This is used in drag and drop operations to store information about the asset in question
	struct AssetPayloadType {
		char name[26];
		char path [100];
		AssetRawType assetType;
	};

	struct GameObjectPayload {
		char name[26];
		std::shared_ptr<GameObject> gameObject;
		AssetRawType assetType;
	};

	class OnSelectGameObjectEvent:public Event  {
	public:
		OnSelectGameObjectEvent(GameObject& gameObj) {
			eCategory = EventCategory::CustomCategory;
			gameObjectSelected = gameObj;
		}

		std::string GetName() const override {
			return std::string("OnSelectGameObject");
		}
		
		SET_EVENT_TYPE(SelectGameObjectEvent);

		GameObject GetSelectedObject()const { return gameObjectSelected; }
		private:
			GameObject gameObjectSelected;
	};

	class OnSelectWindowEvent :public Event {
	public:
		OnSelectWindowEvent(ActiveWindow newWindow):activeWindow(newWindow){
			eCategory = EventCategory::CustomCategory;
		}
		std::string GetName() const override {
			return std::string("OnSelectWindow");
		}

		ActiveWindow GetWindow() const { return activeWindow; }
		SET_EVENT_TYPE(SelectWindowEvent);
	private:
		ActiveWindow activeWindow;
	};

	class OnCreateGameObjectEvent :public Event {
	public:
		OnCreateGameObjectEvent(){
			eCategory = EventCategory::CustomCategory;
		}
		std::string GetName() const override {
			return std::string("CreateGameObjectEvent");
		}

		SET_EVENT_TYPE(CreateGameObjectEvent);
	private:

	};

	class OnDesroyGameObjectEvent :public Event {
	public:
		OnDesroyGameObjectEvent(GameObject& gameObject):gameObject(gameObject) {
			eCategory = EventCategory::CustomCategory;
		}
		std::string GetName() const override {
			return std::string("DestroyGameObjectEvent");
		}

		SET_EVENT_TYPE(DestroyGameObjectEvent);

		GameObject& GetGameObject() { return gameObject; }
	private:
		GameObject gameObject;
	};

	class OnStartRuntimeEvent :public Event {
	public:
		OnStartRuntimeEvent() {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnStartGameEvent");
		}

		SET_EVENT_TYPE(StartGameInEditor);

	private:

	};

	class OnEndRuntimeEvent :public Event {
	public:
		OnEndRuntimeEvent() {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnEndGameEvent");
		}

		SET_EVENT_TYPE(EndGameInEditor);

	private:

	};

	class OnPauseRuntimeEvent :public Event {
	public:
		OnPauseRuntimeEvent() {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnPauseGameEvent");
		}

		SET_EVENT_TYPE(PauseGameInEditor);

	private:

	};
}