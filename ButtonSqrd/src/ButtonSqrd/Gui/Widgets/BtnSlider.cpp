#include "BtnSlider.h"

namespace BtnSqd {
	Mesh& BtnSlider::Draw(std::shared_ptr<Shader> shader) {
		return *sliderMesh;
	}
	std::vector<Vertices> BtnSlider::GenerateVerts() {
		return std::vector<Vertices>();
	}
	void BtnSlider::UpdateMesh() {}
}
