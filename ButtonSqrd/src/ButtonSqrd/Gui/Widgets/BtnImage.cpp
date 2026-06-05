#include "BtnImage.h"

namespace BtnSqd {
	std::vector<unsigned int> BtnImage::indices = {};

	BtnImage::BtnImage() {
		lastDimensions = glm::vec2(0.0f);
		margin = 10.0f;
		color = glm::vec4(1.0f);
		mix = false;
		rect = BtnSmartRect(width, height);
		wType = BtnWidgetType::BtnImage;
		name = "BtnImage";

		InitIndices();
	}

	Mesh& BtnImage::Draw() {
		if (width != lastDimensions.x || height != lastDimensions.y) {
			if (!imageTexture) {
				rect.UpdateDimensions(width, height);
			}

			UpdateMesh();
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
			auto [texWidth, texHeight] = imageTexture->GetResolution();
			rect = BtnSmartRect(texWidth,texHeight);
		}
		else {
			imageTexture = nullptr;
			rect = BtnSmartRect(width,height);
		}
	}

	void BtnImage::InitIndices() {
		if (!BtnImage::indices.empty()) {
			return;
		}

		BtnImage::indices.reserve(54);

		const unsigned int verticesPerSide = 4;

		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int col = 0; col < 3; ++col) {

				unsigned int topLeft = row * verticesPerSide + col;
				unsigned int topRight = topLeft + 1;
				unsigned int bottomLeft = topLeft + verticesPerSide;
				unsigned int bottomRight = bottomLeft + 1;

				BtnImage::indices.push_back(topRight);
				BtnImage::indices.push_back(bottomLeft);
				BtnImage::indices.push_back(topLeft);

				BtnImage::indices.push_back(bottomRight);
				BtnImage::indices.push_back(bottomLeft);
				BtnImage::indices.push_back(topRight);
			}
		}
	}
	std::vector<Vertices> BtnImage::GenerateVerts() {
		std::vector<Vertices> verts;
		verts.reserve(16);

		RectSlicePoints points = rect.GetSlicePoints();
		
		float xPoints[4] = {
			0.0f,
			points.sliceUL.x,
			points.sliceUR.x,
			width
		};
		float yPoints[4] = {
			0.0f,
			points.sliceUL.y,
			points.sliceLL.y,
			height
		};

		float uPos[4] = {
			0.0f,
			rect.GetSlicePercentages().verticalLeft,
			rect.GetSlicePercentages().verticalRight,
			1.0f
		};
		float vPos[4] = {
			0.0f,
			rect.GetSlicePercentages().horizUp,
			rect.GetSlicePercentages().horizDown,
			1.0f
		};

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				verts.push_back({glm::vec3(xPoints[x],yPoints[y],0.0f),{uPos[x],vPos[y]}});
			}
		}

		return verts;
	}
}
