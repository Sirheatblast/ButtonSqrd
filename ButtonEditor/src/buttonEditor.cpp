#include<ButtonSqrd.h>
#include<ButtonSqrd/Core/EntrePoint.h>

#include "EditorInterface.h"

namespace BtnSqd {

	class ButtonEditor :public BtnSqd::Application {
	public:
		ButtonEditor() {
			PushLayer(new EditorInterface());
		}

		~ButtonEditor() {

		}
	};

	BtnSqd::Application* BtnSqd::CreateApp() {

		return new ButtonEditor();
	}
}