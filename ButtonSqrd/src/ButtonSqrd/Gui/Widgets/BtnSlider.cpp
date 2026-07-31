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
		resizeWithBody = true;
		usesSteps = false;

		padding = 0.0f;
		sliderPercentage = 0.0f;
		hasSliderTexture = false;
		maxRange = 1.0f;
		minRange = 0.0f;
		numSteps = 2;
		stepSize = 0.5f;
		mixSlider = false;
		sliderColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
		sliderDimensions = glm::vec2(0.1f, 1.0f);
		sliderSize = glm::vec2(20.0f,20.0f);
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

		float sliderBack=0.0f;
		float sliderWidth=0.0f;
		float sliderHight=0.0f;
		float sliderTop=0.0f;

		float realWidth = (resizeWithBody) ? width*sliderDimensions.x: sliderSize.x;
		float realHeight = (resizeWithBody) ? height*sliderDimensions.y: sliderSize.y;

		if (sDir == SliderDirection::XAxis) {
			float sliderFullArea = realWidth;
			float sliderMax = width - padding;
			float sliderBackMax = (sliderMax - sliderFullArea);
			float sliderPos = (width - padding) * sliderPercentage;

			if (sType == SliderType::Dot) {
				sliderBack = sliderPos - realWidth / 2.0f;
				sliderWidth = sliderPos + realWidth / 2.0f;
				sliderBack = glm::clamp(sliderBack, padding, sliderBackMax);
				sliderWidth = glm::clamp(sliderWidth, realWidth + padding, sliderMax);
			}
			else {
				sliderBack = padding;
				sliderWidth = glm::clamp(sliderPos, padding, sliderMax);
			}

			sliderHight = height / 2.0f + (realHeight*0.5f);
			sliderTop = height/2.0f - (realHeight * 0.5f);			
		}
		else {
			float sliderFullArea = realHeight;
			float sliderMax = height - padding;
			float sliderTopMax = sliderMax - sliderFullArea;			
			float sliderPos = (height - padding) * sliderPercentage;

			if (sType == SliderType::Dot) {
				sliderTop = sliderPos - realHeight / 2.0f;
				sliderHight = sliderPos + realHeight / 2.0f;

				sliderTop = glm::clamp(sliderTop, padding, sliderTopMax);
				sliderHight = glm::clamp(sliderHight, realHeight + padding, sliderMax);
			}
			else {
				sliderTop = padding;
				sliderHight = glm::clamp(sliderPos, padding, sliderMax);
			}

			sliderWidth = width / 2.0f + (realWidth*0.5f);
			sliderBack = width / 2.0f - (realWidth * 0.5f);

			sliderHight = glm::clamp(sliderHight, 0.0f, sliderMax);
			sliderTop = glm::clamp(sliderTop, 0.0f, sliderTopMax);
		}

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
	void BtnSlider::SetSliderPercentage(float percent) {
		float nPercentage = percent;
		if (usesSteps) {
			float steps = static_cast<float>(numSteps - 1);
			if (steps <= 0.0f) {
				steps = 1.0f;
			}
			nPercentage = std::roundf(percent * steps) / steps;
		}

		shouldRemake = true;
		sliderPercentage = nPercentage;
	}
	const float BtnSlider::GetSliderValue() {
		float value = maxRange - minRange;
		value *= sliderPercentage;
		return minRange + value;
	}
}
