#pragma once
#include"ButtonSqrd/Core/core.h"
#include"ButtonSqrd/Core/Input.h"
#include"Event.h"

#include<string>

namespace BtnSqd {

	//Events
	class OnKeyDownEvent :public Event {
	public:
		OnKeyDownEvent(KeyCode m_keycode):keycode(m_keycode){eCategory = EventCategory::KeyEventType;}

		std::string GetName() const override {
			return "KeyDownEvent: " + std::to_string(char(keycode))+" pressed";
		}

		KeyCode GetKey()const { return keycode; }

		SET_EVENT_TYPE(KeyDownEvent);
	private:
		KeyCode keycode;
	};

	class OnKeyReleaseEvent :public Event {
	public:
		OnKeyReleaseEvent(KeyCode m_keycode) :keycode(m_keycode) { eCategory = EventCategory::KeyEventType; }

		std::string GetName() const override {
			return "KeyReleaseEvent: " + std::to_string(char(keycode)) + " released";
		}

		KeyCode GetKey()const { return keycode; }

		SET_EVENT_TYPE(KeyReleaseEvent);
	private:
		KeyCode keycode;
	};

	class OnKeyHoldEvent :public Event {
	public:
		OnKeyHoldEvent(KeyCode m_keycode) :keycode(m_keycode) { eCategory = EventCategory::KeyEventType; }

		std::string GetName() const override {
			return "KeyHoldEvent: " + std::to_string(char(keycode)) + " held";
		}

		inline KeyCode GetKey() { return keycode; }

		SET_EVENT_TYPE(KeyHoldEvent);
	private:
		KeyCode keycode;
	};

	class OnKeyTypedEvent :public Event {
	public:
		OnKeyTypedEvent(char key):key(key){}

		std::string GetName() const override {
			return "KeyTypedEvent: " + std::to_string(char(key)) + " held";
		}

		unsigned int GetKey() { return key; }

		SET_EVENT_TYPE(keyTypedEvent);
	private:
		char key;
	};
}