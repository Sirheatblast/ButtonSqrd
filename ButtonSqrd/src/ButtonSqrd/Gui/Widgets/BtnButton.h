#pragma once
#include<ButtonSqrd/Gui/BtnWidget.h>

namespace BtnSqd {
	class BtnButton:public BtnWidget {
	public:
		BtnButton();
		Mesh& Draw(std::shared_ptr<Shader> shader) override;
		void OnUpdate()override;
		void OnClick()override;
		void OnClickUp()override;
	private:
		std::shared_ptr<Mesh>imageMesh;
	};
}