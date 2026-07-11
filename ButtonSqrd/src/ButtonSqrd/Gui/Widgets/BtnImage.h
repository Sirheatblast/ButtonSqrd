#pragma once
#include"ButtonSqrd/Gui/BtnWidget.h"
#include"ButtonSqrd/Render/Mesh.h"
#include"ButtonSqrd/Core/ResourceManager.h"
#include<ButtonSqrd/Gui/BtnSmartRect.h>
#include<glm/glm.hpp>
#include<vector>

namespace BtnSqd {
	class BtnImage:public BtnWidget {
	public:
		BtnImage();

		Mesh& Draw(std::shared_ptr<Shader> shader) override;
		void UpdateMesh();
		void SetTexture(std::string path);
		std::shared_ptr<Texture> GetTexture() { return imageTexture; }

	private:
		std::vector<Vertices> GenerateVerts();

		static inline std::vector<unsigned int> indices = { 0,2,1,2,0,3 };

		std::shared_ptr<Mesh>imageMesh;
		std::shared_ptr<Texture>imageTexture;
		glm::vec2 lastDimensions;
		BtnSmartRect rect;
	};
}