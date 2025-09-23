#pragma once
#include"ButtonSqrd/Core/core.h"
#include"ButtonSqrd/Core/Input.h"
#include"Event.h"

namespace BtnSqd {
	
	//Events
	class OnMouseClickEvent :public Event {
	public:
		OnMouseClickEvent(int button) :button(button) { eCategory = EventCategory::MouseEventType; }

		std::string GetName() const override {
			return "MouseClickEvent: " + std::to_string(char(button)) + " pressed";
		}

		int GetButton() const { return button; }

		SET_EVENT_TYPE(MouseClickEvent);
	private:
		int button;
	};

	class OnMouseClickReleaseEvent :public Event {
	public:
		OnMouseClickReleaseEvent(int button) :button(button) { eCategory = EventCategory::MouseEventType; }

		std::string GetName() const override {
			return "MouseClickReleaseEvent: " + std::to_string(char(button)) + " released";
		}

		int GetButton() const { return button; }

		SET_EVENT_TYPE(MouseClickReleaseEvent);
	private:
		int button;
	};

	class OnMouseMoveEvent :public Event {
	public:
		OnMouseMoveEvent(float m_posX,float m_posY){
			position = std::make_pair(m_posX, m_posY);
			eCategory = EventCategory::MouseEventType;
		}

		std::string GetName() const override {
			return "MouseMoveEvent: " + std::to_string(position.first)+","+std::to_string(position.second);
		}

		std::pair<float, float> GetPosition()const {
			return position;
		}

		SET_EVENT_TYPE(MouseMoveEvent);
	private:
		std::pair<float, float> position;
	};

	class OnMouseScrollEvent :public Event {
	public:
		OnMouseScrollEvent(float m_offsetX, float m_offsetY) :offsetX(m_offsetX), offsetY(m_offsetY) { eCategory = EventCategory::MouseEventType; }

		std::string GetName() const override {
			return "MouseOffsetEvent: " + std::to_string(offsetX) + "," + std::to_string(offsetY);
		}

		std::pair<float, float> GetOffset(){
			return std::make_pair(offsetX, offsetY);
		}

		SET_EVENT_TYPE(MouseScrollEvent);
	private:
		float offsetX, offsetY;
	};
}