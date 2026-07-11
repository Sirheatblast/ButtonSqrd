#include "BtnImage.h"

namespace BtnSqd {
	BtnImage::BtnImage() {
		lastDimensions = glm::vec2(0.0f);
		textureScale = 1.0f;
		color = glm::vec4(1.0f);
		mix = false;
		wType = BtnWidgetType::BtnImage;
		name = "BtnImage";
		useNineSlice = false;
	}

	Mesh& BtnImage::Draw(std::shared_ptr<Shader> shader) {
		if (width != lastDimensions.x || height != lastDimensions.y) {
			UpdateMesh();
		}

		glm::vec2 texSize = glm::vec2(0.0f);

		if (imageTexture) {
			imageMesh->SetTexture(imageTexture);
			rect.SetSlicePoints(imageTexture->GetSlices());
			auto [tWidth,tHeight] = imageTexture->GetResolution();
			texSize = { (float)tWidth,(float)tHeight };
		}
		RectSlicePercentages slices = rect.GetSlicePercentages();
		RectSlicePoints slicePoints = rect.GetSlicePoints();
		shader->SetVec4("sliceBounds", {slices.verticalLeft,slices.verticalRight,slices.horizUp,slices.horizDown});
		shader->SetVec4("slicePos", { slicePoints.sliceUL,slicePoints.sliceUR,slicePoints.sliceLL,slicePoints.sliceLR});
		shader->SetVec2("texSize",texSize);
		shader->SetBool("useNineSlice", useNineSlice);
		shader->SetFloat("textureScale", textureScale);

		return *imageMesh;
	}

	void BtnImage::UpdateMesh() {
		lastDimensions.x = width;
		lastDimensions.y = height;

		std::vector<Vertices> verts = GenerateVerts();
		imageMesh.reset(new Mesh(verts, indices, Material()));
	}

	void BtnImage::SetTexture(std::string path) {
		if (ResourceManager::GetLoadedTextures().contains(path)) {
			imageTexture = ResourceManager::GetLoadedTextures()[path];
			hasTexture = true;
		}
		else {
			imageTexture = nullptr;
			hasTexture = false;
		}
		rect = BtnSmartRect(width, height);
	}

	std::vector<Vertices> BtnImage::GenerateVerts() {
		std::vector<Vertices> verts;
		verts.reserve(4);

		verts.push_back({ glm::vec3(0.0f,   0.0f,    0.0f), glm::vec2(0.0f, 0.0f) });
		verts.push_back({ glm::vec3(width,  0.0f,    0.0f), glm::vec2(1.0f, 0.0f) });
		verts.push_back({ glm::vec3(width,  height,  0.0f), glm::vec2(1.0f, 1.0f) });
		verts.push_back({ glm::vec3(0.0f,   height,  0.0f), glm::vec2(0.0f, 1.0f) });

		return verts;
	}
}
