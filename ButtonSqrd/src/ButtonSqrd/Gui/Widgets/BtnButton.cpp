#include "BtnButton.h"

namespace BtnSqd {
	BtnButton::BtnButton() {
		
	}

	Mesh& BtnButton::Draw(std::shared_ptr<Shader> shader) {
		return *imageMesh;
	}
	void BtnButton::OnUpdate() {
	
	}
	void BtnButton::OnClick() {
	
	}
	void BtnButton::OnClickUp() {
	
	}
}
