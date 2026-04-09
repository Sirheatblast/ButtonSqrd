#pragma once
#include"Event.h"

namespace BtnSqd {
	class OnEnableGuiEvent :public Event {
	public:
		OnEnableGuiEvent(std::string dest = "def") :dest(dest) {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnEnableGuiEvent");
		}

		std::string GetDest()const { return dest; }

		SET_EVENT_TYPE(EnableGui);

	private:
		std::string dest;
	};

	class OnDisableGuiEvent :public Event {
	public:
		OnDisableGuiEvent(std::string dest = "def") :dest(dest) {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnDisableGuiEvent");
		}
		std::string GetDest()const { return dest; }

		SET_EVENT_TYPE(DisableGui);

	private:
		std::string dest;
	};

	class OnEditWidgetEnableEvent :public Event {
	public:
		OnEditWidgetEnableEvent(std::string dest = "def") :dest(dest) {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnDisableGuiEvent");
		}
		std::string GetDest()const { return dest; }

		SET_EVENT_TYPE(EditWidgetEnable);

	private:
		std::string dest;
	};

	class OnEditWidgetDisableEvent :public Event {
	public:
		OnEditWidgetDisableEvent(std::string dest = "def") :dest(dest) {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnDisableGuiEvent");
		}
		std::string GetDest()const { return dest; }

		SET_EVENT_TYPE(EditWidgetDisable);

	private:
		std::string dest;
	};

	class OnGetWidgetEvent :public Event {
	public:
		OnGetWidgetEvent(std::shared_ptr<BtnWidget> widget) :widget(widget) {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnGetWidgetEvent");
		}
		std::shared_ptr<BtnWidget> GetWidget()const { return widget; }

		SET_EVENT_TYPE(GetWidget);

	private:
		std::shared_ptr<BtnWidget> widget;
	};

	class OnSelectWidgetEvent :public Event {
	public:
		OnSelectWidgetEvent(std::shared_ptr<BtnWidget> widget) :widget(widget) {
			eCategory = EventCategory::CustomCategory;
		}

		std::string GetName() const override {
			return std::string("OnSelectWidgetEvent");
		}
		std::shared_ptr<BtnWidget> GetWidget()const { return widget; }

		SET_EVENT_TYPE(SelectWidget);

	private:
		std::shared_ptr<BtnWidget> widget;
	};
}