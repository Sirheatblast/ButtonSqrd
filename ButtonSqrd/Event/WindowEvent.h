#pragma once

#include"../Core/core.h"
#include"Event.h"
#include"../Core/Logger.h"

#include<string>

namespace BtnSqd {

	class OnWindowResizeEvent: public Event {
	public:
		OnWindowResizeEvent(unsigned int m_width, unsigned int m_height,int m_priority): width(m_width), height(m_height), priority(m_priority) { eCategory = EventCategory::WindowEventType; }

		inline unsigned int GetWidth() const{ return width; }
		inline unsigned int GethHeight() const { return height; }
		std::string GetName() const override {
			std::string m_name;
			m_name = "WindowResizeEvent:" + std::to_string(width) + "," + std::to_string(height);
			return m_name;
		}

		SET_EVENT_TYPE(WindowResizeEvent);
	private:
		unsigned int height, width;
		int priority;
	};
	
	class OnWindowCloseEvent : public Event {
	public:
		OnWindowCloseEvent(int m_priority):priority(m_priority) { eCategory = EventCategory::WindowEventType; }
		std::string GetName() const override {
			std::string m_name;
			m_name = "WindowCloseEvent";
			return m_name;
		}
		SET_EVENT_TYPE(WindowCloseEvent);

	private:
		int priority;
	};
	
	class OnWindowMoveEvent : public Event {
	public:
		OnWindowMoveEvent(int m_posX,int m_posY):posX(m_posX),posY(m_posY){ eCategory = EventCategory::WindowEventType; }
		std::string GetName() const override {
			std::string m_name;
			m_name = "WindowMoveEvent: " + std::to_string(posX) + "," + std::to_string(posY);
			return m_name;
		};
		SET_EVENT_TYPE(WindowMoveEvent);

	private:
		int posX, posY;
	};
}