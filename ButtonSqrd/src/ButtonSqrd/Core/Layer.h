#pragma once
#include"core.h"
#include"ButtonSqrd/Event/Event.h"

namespace BtnSqd {
	class Layer {
	public:

		virtual ~Layer();
		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEvent(Event* e);

		void SetEnabled(bool enable) { isEnabled = enable; }
		bool Enabled()const { return isEnabled; }

	private:
		bool isEnabled=true;
	};
}