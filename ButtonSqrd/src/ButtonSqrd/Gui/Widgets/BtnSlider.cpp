#include "BtnSlider.h"

namespace BtnSqd {
	BtnSlider::BtnSlider() {
		lastDimensions = glm::vec2(0.0f);
		textureScale = 1.0f;
		color = glm::vec4(1.0f);
		mix = false;
		wType = BtnWidgetType::Slider;
		useNineSlice = false;
		width = 200.0f;
		height = 25.0f;
		name = "New Slider";
		isSliderClick = false;
		isSliderHover = false;
		isInteractive = true;
		shouldRemake = true;

		sliderPercentage = 0.0f;
		hasSliderTexture = false;
		maxRange = 1.0f;
		minRange = 0.0f;
		mixSlider = false;
		sliderColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		sliderDimensions = glm::vec2(0.1f, 1.0f);
		sType = SliderType::Dot;
		sDir = SliderDirection::XAxis;
	}

	Mesh& BtnSlider::Draw(std::shared_ptr<Shader> shader) {
		if (width != lastDimensions.x || height != lastDimensions.y 
			|| shouldRemake){
			shouldRemake = false;
			UpdateMesh();
		}

		glm::vec2 texSize = glm::vec2(0.0f);

		if (bodyTexture) {
			bodyMesh->SetTexture(bodyTexture);
			rect.SetSlicePoints(bodyTexture->GetSlices());
			auto [tWidth, tHeight] = bodyTexture->GetResolution();
			texSize = { (float)tWidth,(float)tHeight };
		}

		RectSlicePercentages slices = rect.GetSlicePercentages();
		RectSlicePoints slicePoints = rect.GetSlicePoints();
		shader->SetVec4("sliceBounds", { slices.verticalLeft,slices.verticalRight,slices.horizUp,slices.horizDown });
		shader->SetVec4("slicePos", { slicePoints.sliceUL,slicePoints.sliceUR,slicePoints.sliceLL,slicePoints.sliceLR });
		shader->SetVec2("texSize", texSize);
		shader->SetBool("useNineSlice", useNineSlice);
		shader->SetFloat("textureScale", textureScale);

		return *bodyMesh;
	}
	Mesh& BtnSlider::DrawSlider(std::shared_ptr<Shader> shader) {
		glm::vec2 texSize = glm::vec2(0.0f);

		if (sliderTexture) {
			sliderMesh->SetTexture(sliderTexture);
			rect.SetSlicePoints(sliderTexture->GetSlices());
			auto [tWidth, tHeight] = sliderTexture->GetResolution();
			texSize = { (float)tWidth,(float)tHeight };
		}

		RectSlicePercentages slices = rect.GetSlicePercentages();
		RectSlicePoints slicePoints = rect.GetSlicePoints();
		shader->SetVec4("sliceBounds", { slices.verticalLeft,slices.verticalRight,slices.horizUp,slices.horizDown });
		shader->SetVec4("slicePos", { slicePoints.sliceUL,slicePoints.sliceUR,slicePoints.sliceLL,slicePoints.sliceLR });
		shader->SetVec2("texSize", texSize);
		shader->SetBool("useNineSlice", useNineSlice);
		shader->SetFloat("textureScale", textureScale);

		return *sliderMesh;
	}

	void BtnSlider::SetSliderTexture(std::shared_ptr<Texture> nSliderTex) {}

	glm::vec2 BtnSlider::GetSliderDimensions() {
		return sliderDimensions;
	}

	std::vector<Vertices> BtnSlider::GenerateBodyVerts() {
		std::vector<Vertices> verts;
		verts.reserve(4);

		verts.push_back({ glm::vec3(0.0f,   0.0f,    0.0f), glm::vec2(0.0f, 0.0f) });
		verts.push_back({ glm::vec3(width,  0.0f,    0.0f), glm::vec2(1.0f, 0.0f) });
		verts.push_back({ glm::vec3(width,  height,  0.0f), glm::vec2(1.0f, 1.0f) });
		verts.push_back({ glm::vec3(0.0f,   height,  0.0f), glm::vec2(0.0f, 1.0f) });

		return verts;
	}
	std::vector<Vertices> BtnSlider::GenerateSliderVerts() {
		std::vector<Vertices> verts;
		verts.reserve(4);

		float sliderFullWidth = (width * sliderDimensions.x);
		float sliderMax = width;
		float sliderBackMax = sliderMax - sliderFullWidth;

		float sliderBack = (sType==SliderType::Dot) ? 0.0f + (width * sliderPercentage):0.0f;
		float sliderWidth = (sType == SliderType::Dot) ? sliderBack + sliderFullWidth: (width * sliderPercentage);
		float sliderHight =height* sliderDimensions.y;
		float sliderTop = height - (height * sliderDimensions.y);

		sliderWidth = glm::clamp(sliderWidth, 0.0f, sliderMax);
		sliderBack = glm::clamp(sliderBack, 0.0f, sliderBackMax);

		verts.push_back({ glm::vec3(sliderBack,   sliderTop,    0.0f), glm::vec2(0.0f, 0.0f) });
		verts.push_back({ glm::vec3(sliderWidth,  sliderTop,    0.0f), glm::vec2(1.0f, 0.0f) });
		verts.push_back({ glm::vec3(sliderWidth,  sliderHight,  0.0f), glm::vec2(1.0f, 1.0f) });
		verts.push_back({ glm::vec3(sliderBack,   sliderHight,  0.0f), glm::vec2(0.0f, 1.0f) });

		return verts;
	}
	void BtnSlider::UpdateMesh() {
		lastDimensions.x = width;
		lastDimensions.y = height;
		lastDimensions.y = height;

		std::vector<Vertices> verts = GenerateBodyVerts();
		bodyMesh.reset(new Mesh(verts, indices, Material()));

		std::vector<Vertices> sVerts = GenerateSliderVerts();
		sliderMesh.reset(new Mesh(sVerts, indices, Material()));
	}
}
