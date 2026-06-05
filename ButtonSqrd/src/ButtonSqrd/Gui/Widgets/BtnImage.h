#pragma once
#include"ButtonSqrd/Gui/BtnWidget.h"
#include"ButtonSqrd/Gui/BtnSmartRect.h"
#include"ButtonSqrd/Render/Mesh.h"
#include"ButtonSqrd/Core/ResourceManager.h"

#include<glm/glm.hpp>
#include<vector>

namespace BtnSqd {
	class BtnImage:public BtnWidget {
	public:
		BtnImage();

		Mesh& Draw() override;
		void UpdateMesh();
		void SetTexture(std::string path);

	private:
		void InitIndices();
		std::vector<Vertices> GenerateVerts();

		std::vector<unsigned int>indices;
		BtnSmartRect rect;

		std::shared_ptr<Mesh>imageMesh;
		std::shared_ptr<Texture>imageTexture;
		glm::vec2 lastDimensions;
		glm::vec4 color;
		bool mix;
		float margin;
	};
}