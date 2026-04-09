#pragma once
#include"ButtonSqrd/Core/core.h"
#include"ButtonSqrd/Core/Scene.h"
#include"Event.h"

#include<string>
#include<memory>


namespace BtnSqd {
	class OnChangeSceneEvent :public Event {
	public:
		OnChangeSceneEvent(BtnScene* nScene) {
			eCategory = EventCategory::CustomCategory;
			scene.reset(nScene);
		}

		std::string GetName() const override {
			return std::string("OnChangeSceneEvent");
		}

		SET_EVENT_TYPE(ChangeScene);
		std::shared_ptr<BtnScene> GetScene()const { return scene; }

	private:
		std::shared_ptr<BtnScene>scene;
	};
}