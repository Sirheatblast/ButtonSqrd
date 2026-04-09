#pragma once
#include"ButtonSqrd/Gui/BtnWidget.h"

#include<string>

namespace BtnSqd {
	class BtnTextBox :public BtnWidget {
	public:
		BtnTextBox();
		~BtnTextBox() override;

		Mesh& Draw() override;

		void SetVerts();

	private:
		static std::vector<unsigned int>indices;
		
		unsigned int lastWidth=0;
		unsigned int lastHeight=0;

		std::vector<Vertices> verts;
		std::shared_ptr<Mesh> textMesh;

		std::string text;
	};
}