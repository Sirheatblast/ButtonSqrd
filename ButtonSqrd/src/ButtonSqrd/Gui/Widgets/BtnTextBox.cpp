#include "BtnTextBox.h"

namespace BtnSqd {
	std::vector<unsigned int> BtnSqd::BtnTextBox::indices = {
		0, 3, 4,
		0, 4, 1,
		1, 4, 5,
		1, 5, 2,
		3, 6, 7,
		3, 7, 4,
		4, 7, 8,
		4, 8, 5
	};

	BtnTextBox::BtnTextBox() {
		name = "new TextBox";
		width = 200.0f;
		height = 50.0f;
		border = 15.0f;
		wType = BtnWidgetType::Text;

		SetVerts();
	}

	BtnTextBox::~BtnTextBox() {

	}

	Mesh& BtnSqd::BtnTextBox::Draw() {
		SetVerts();
		return *textMesh;
	}
	void BtnTextBox::SetVerts() {
		if (lastWidth != width || lastHeight != height) {
			verts = {
				{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } }, // 0: Top-Left
				{ { border,    0.0f, 0.0f },{ 0.1f, 0.0f } }, // 1: Top-Inner Left
				{ { width,    0.0f, 0.0f },{ 1.0f, 0.0f } }, // 2: Top-Right

				{ { 0.0f, border,    0.0f },{ 0.0f, 0.1f } }, // 3: Mid-Inner Top
				{ { border,    border,    0.0f },{ 0.1f, 0.1f } }, // 4: Center
				{ { width,    border,    0.0f },{ 1.0f, 0.1f } }, // 5: Mid-Inner Right

				{ { 0.0f,height,    0.0f },{ 0.0f, 1.0f } }, // 6: Bottom-Left
				{ { border,   height,    0.0f },{ 0.1f, 1.0f } }, // 7: Bottom-Inner Left
				{ { width,   height,    0.0f },{ 1.0f, 1.0f } }  // 8: Bottom-Right
			};
			lastWidth = width;
			lastHeight = height;
			textMesh = std::make_shared<Mesh>(verts, indices, Material());
		}
	}
}

