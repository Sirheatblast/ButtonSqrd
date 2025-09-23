#pragma once

#include"core.h"
#include"ButtonSqrd/Event/WindowEvent.h"

#include<functional>

//Interface
namespace BtnSqd {

	class Window {

	public:
		using EventCallbackFn = std::function<void(Event*)>;

		inline virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		inline virtual void SetCursorLock(bool locked)=0;
		inline virtual bool GetCursorLock()=0;

		virtual void SetAsCurrent() = 0;

		virtual std::pair<int, int>GetResolution() =0;

		virtual void OnUpdate() = 0;
		virtual void UpdateImGui() = 0;
		virtual void DrawImGuiFrame() = 0;
		virtual Window* GetWindow() = 0;
		virtual bool ShouldClose() = 0;

		virtual void* GetNativeWindow() = 0;

		static Window* CreateWindowFunction(const char* m_name, bool isLocked=false, int m_height=-1, int m_width=-1);
	};


}