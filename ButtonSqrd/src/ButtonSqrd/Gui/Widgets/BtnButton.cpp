#include "BtnButton.h"

namespace BtnSqd {
	BtnButton::BtnButton() {
		lastDimensions = glm::vec2(0.0f);
		textureScale = 1.0f;
		color = glm::vec4(1.0f);
		mix = false;
		wType = BtnWidgetType::Button;
		useNineSlice = false;
		width = 200.0f;
		height = 50.0f;

		name = "New Button";
		hasHoverTexture = false;
		hasClickTexture = false;
		hoverColor = glm::vec4(1.0f);
		clickColor = glm::vec4(1.0f);

		useClickTexture = false;
		useHoverTexture = false;

		applyHoverColor = true;
		applyClickColor = true;
		isInteractive = true;
	}

	Mesh& BtnButton::Draw(std::shared_ptr<Shader> shader) {
		if (width != lastDimensions.x || height != lastDimensions.y) {
			UpdateMesh();
		}

		glm::vec2 texSize = glm::vec2(0.0f);

		if (buttonTexture) {
			buttonMesh->SetTexture(buttonTexture);
			rect.SetSlicePoints(buttonTexture->GetSlices());
			auto [tWidth, tHeight] = buttonTexture->GetResolution();
			texSize = { (float)tWidth,(float)tHeight };
		}
		RectSlicePercentages slices = rect.GetSlicePercentages();
		RectSlicePoints slicePoints = rect.GetSlicePoints();
		shader->SetVec4("sliceBounds", { slices.verticalLeft,slices.verticalRight,slices.horizUp,slices.horizDown });
		shader->SetVec4("slicePos", { slicePoints.sliceUL,slicePoints.sliceUR,slicePoints.sliceLL,slicePoints.sliceLR });
		shader->SetVec2("texSize", texSize);
		shader->SetBool("useNineSlice", useNineSlice);
		shader->SetFloat("textureScale", textureScale);

		return *buttonMesh;
	}

	void BtnButton::SetMainTexture(std::string path) {
		if (ResourceManager::GetLoadedTextures().contains(path)) {
			buttonTexture = ResourceManager::GetLoadedTextures()[path];
			hasTexture = true;
		}
		else {
			buttonTexture = nullptr;
			hasTexture = false;
		}
		rect = BtnSmartRect(width, height);
	}

	void BtnButton::SetHoverTexture(std::string path) {
		if (ResourceManager::GetLoadedTextures().contains(path)) {
			hoverTexture = ResourceManager::GetLoadedTextures()[path];
			hasHoverTexture = true;
		}
		else {
			hoverTexture = nullptr;
			hasHoverTexture = false;
		}
		rect = BtnSmartRect(width, height);
	}

	void BtnButton::SetClickTexture(std::string path) {
		if (ResourceManager::GetLoadedTextures().contains(path)) {
			clickTexture = ResourceManager::GetLoadedTextures()[path];
			hasClickTexture = true;
		}
		else {
			clickTexture = nullptr;
			hasClickTexture = false;
		}
		rect = BtnSmartRect(width, height);
	}

	void BtnButton::OnUpdate() {
	
	}
	void BtnButton::OnClick() {
	
	}
	void BtnButton::OnClickUp() {
	
	}
	std::vector<Vertices> BtnButton::GenerateVerts() {
		std::vector<Vertices> verts;
		verts.reserve(4);

		verts.push_back({ glm::vec3(0.0f,   0.0f,    0.0f), glm::vec2(0.0f, 0.0f) });
		verts.push_back({ glm::vec3(width,  0.0f,    0.0f), glm::vec2(1.0f, 0.0f) });
		verts.push_back({ glm::vec3(width,  height,  0.0f), glm::vec2(1.0f, 1.0f) });
		verts.push_back({ glm::vec3(0.0f,   height,  0.0f), glm::vec2(0.0f, 1.0f) });

		return verts;
	}
	void BtnButton::UpdateMesh() {
		lastDimensions.x = width;
		lastDimensions.y = height;

		std::vector<Vertices> verts = GenerateVerts();
		buttonMesh.reset(new Mesh(verts, indices, Material()));
	}
}
