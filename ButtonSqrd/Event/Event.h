#pragma once
#include"ButtonSqrd/Core/core.h"

#include<vector>
#include<functional>
#include<iostream>
#include<string>

namespace BtnSqd {
	enum class EventType {
		//Window Events
		WindowResizeEvent, WindowCloseEvent, WindowMoveEvent,
		//KeyBoard Events
		KeyDownEvent, KeyReleaseEvent,KeyHoldEvent,keyTypedEvent,
		//Mouse Events
		MouseMoveEvent,MouseClickEvent,MouseClickReleaseEvent,MouseScrollEvent,
		//EditorEvents
		SelectGameObjectEvent, SelectWindowEvent, CreateGameObjectEvent, DestroyGameObjectEvent, StartGameInEditor,EndGameInEditor,PauseGameInEditor,
		//Camera Events
		SetActiveCamera,
		//Null
		NullEvent
	};

	enum class EventCategory {
		WindowEventType,KeyEventType,MouseEventType,CustomCategory,NullCategory
	};
#define SET_EVENT_TYPE(type) static EventType GetStaticType(){return EventType::##type;}\
							 virtual EventType GetEventType() const override{return GetStaticType();}

	//Interface for creating a new event
	class Event {
		friend class EventDispatcher;
	public:
		virtual std::string GetName() const = 0;
		virtual EventType GetEventType() const = 0;
		EventCategory GetEventCategory() const { return eCategory; }
		bool IsInCategory(EventCategory eventCategory) {
			return eventCategory == eCategory;
		}
		bool isHandled=false;

	protected:
		EventType eType=EventType::NullEvent;
		EventCategory eCategory = EventCategory::NullCategory;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event* event): event(event){}

		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (event->GetEventType() == T::GetStaticType()) {
				event->isHandled |= func(static_cast<T*>(event));
				return true;
			}
			return false;
		}
	private:
		Event* event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.GetName();
	}
}