#pragma once
#include"ButtonSqrd/Core/core.h"
#include"ButtonSqrd/Core/CoreComponents.h"
#include"ButtonSqrd/Core/GameObject.h"
#include"Event.h"

#include<string>

namespace BtnSqd {
	class OnSetActiveCameraEvent :public Event {
	public:
		OnSetActiveCameraEvent(GameObject nCamera):camera(nCamera) { eCategory = EventCategory::CustomCategory; }

		std::string GetName() const override {
			return "Set Active Camera Event";
		}

		GameObject GetCamera() { return camera; }

		SET_EVENT_TYPE(SetActiveCamera);
	private:
		GameObject camera;
	};
}