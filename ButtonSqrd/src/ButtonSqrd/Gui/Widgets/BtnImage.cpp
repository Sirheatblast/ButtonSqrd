#include "BtnImage.h"

namespace BtnSqd {
	BtnImage::BtnImage() {
		lastDimensions = glm::vec2(0.0f);
		margin = 10.0f;
		color = glm::vec4(1.0f);
		mix = false;
		wType = BtnWidgetType::BtnImage;
		name = "BtnImage";
		imageScale = false;
		InitIndices();
	}

	Mesh& BtnImage::Draw() {
		if (width != lastDimensions.x || height != lastDimensions.y) {
			UpdateMesh();
		}

		if (imageTexture) {
			imageMesh->SetTexture(imageTexture);
			imageTexture->GetRect().UpdateDimensions(width, height);
			
		}

		return *imageMesh;
	}

	void BtnImage::UpdateMesh() {
		lastDimensions.x = width;
		lastDimensions.y = height;

		std::vector<Vertices> verts = GenerateVerts();
		imageMesh.reset(new Mesh(verts,indices,Material()));
	}

	void BtnImage::SetTexture(std::string path) {
		if(ResourceManager::GetLoadedTextures().contains(path)){
			imageTexture = ResourceManager::GetLoadedTextures()[path];
			hasTexture = true;
		}
		else {
			imageTexture = nullptr;
			hasTexture = false;
		}
		imageTexture->GetRect() = BtnSmartRect(width, height);
	}

	void BtnImage::SetUseImageScale(bool useScale) {
		imageScale = useScale; 
		UpdateMesh();
	}

	void BtnImage::InitIndices() {
		if (!indices.empty()) {
			return;
		}

		indices.reserve(54);

		const unsigned int verticesPerSide = 4;

		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int col = 0; col < 3; ++col) {

				unsigned int topLeft = row * verticesPerSide + col;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = topLeft + verticesPerSide;
				unsigned int bottomRight = bottomLeft + 1;

				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(topLeft);

				indices.push_back(bottomRight);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);
			}
		}
	}
	std::vector<Vertices> BtnImage::GenerateVerts() {
		std::vector<Vertices> verts;
		verts.reserve(16);

		RectSlicePoints points;
		if (imageTexture) {
			points = imageTexture->GetRect().GetSlicePoints();
		}
		
		float xPoints[4] = { 0.0f, points.sliceUL.x, points.sliceUR.x, width };
		float yPoints[4] = { 0.0f, points.sliceUL.y,points.sliceLL.y, height };

		float uRatio = 1.0f;
		float vRatio = 1.0f;
		if (imageScale&&imageTexture) {
			auto [texWidth,texHeight] = imageTexture->GetResolution();
			uRatio = (float)width / texWidth;
			vRatio = (float)height / texWidth;
		}

		RectSlicePercentages slices;
		if (imageTexture) {
			slices = imageTexture->GetRect().GetSlicePercentages();
		}

		float uPos[4] = { 0.0f, slices.verticalLeft*uRatio, slices.verticalRight*uRatio, uRatio };
		float vPos[4] = { 0.0f, slices.horizUp* vRatio, slices.horizDown* vRatio,vRatio };

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				verts.push_back({glm::vec3(xPoints[x],yPoints[y],0.0f),{uPos[x],vPos[y]}});
			}
		}

		return verts;
	}
}
