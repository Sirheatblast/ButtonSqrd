#pragma once
#include<ButtonSqrd/Gui/BtnWidget.h>
#include<ButtonSqrd/Gui/BtnSmartRect.h>

#include<functional>
namespace BtnSqd {
	enum SliderType {
		Dot = 0,
		Bar,
	};

	enum SliderDirection {
		XAxis = 0,
		YAxis
	};

	class BtnSlider : public BtnWidget {
	public:
		BtnSlider();

		Mesh& Draw(std::shared_ptr<Shader> shader) override;
		Mesh& DrawSlider(std::shared_ptr<Shader>shader);
		glm::vec2 GetSliderSize() {
			return sliderSize;
		}
		void SetSliderSize(glm::vec2 sSize) {
			shouldRemake = true;
			sliderSize = sSize;
		}

		glm::vec2 GetSliderDimensions();
		void SetSliderDimension(glm::vec2 sliderDim) {
			shouldRemake = true;
			sliderDimensions = sliderDim;
			SetSliderSize(glm::vec2(width*sliderDim.x,height*sliderDim.y));
		}

		const SliderDirection GetDirection(){return sDir;}
		void SetSliderDirection(SliderDirection dir) {
			shouldRemake = true;
			sDir = dir;
		}

		const float GetPadding() { return padding; }
		void SetPadding(float nPadding){
			shouldRemake = true;
			padding = nPadding;
		}

		bool GetResizeWithBody() {
			return resizeWithBody;
		}

		void SetResizeWithBody(bool shouldResize) {
			SetSliderDimension(sliderDimensions);
			resizeWithBody = shouldResize;
		}

		const glm::vec4 GetSliderColor() { return sliderColor; }
		void SetSliderColor(glm::vec4 nColor) { sliderColor = nColor; }

		void SetSliderPercentage(float percent);
		const float GetSliderPercentage() { return sliderPercentage; }

		const bool GetHasSliderTexture() { return hasSliderTexture; }
		void SetSliderTexture(std::shared_ptr<Texture>nSliderTex);

		const bool GetMixSlider() { return mixSlider; }
		void SetMixSlider(bool mix) { mixSlider = mix; }

		const SliderType GetSliderType() { return sType; }
		void SetSliderType(SliderType nSType) {
			shouldRemake = true;
			sType = nSType;
		}

		const float GetMinRange() { return minRange; }
		void SetMinRange(float min) { minRange = min; }

		const float GetMaxRange() { return maxRange; }
		void SetMaxRange(float max) { maxRange = max; }

		const bool GetUseSteps() { return usesSteps; }
		void SetUseSteps(bool useStep) { usesSteps = useStep; }

		const unsigned int GetNumSteps() { return numSteps; }
		void SetNumSteps(unsigned int steps) {
			numSteps = steps;
			stepSize = 1.0f / static_cast<float>(steps);
		}

	private:
		std::vector<Vertices> GenerateBodyVerts();
		std::vector<Vertices> GenerateSliderVerts();
		void UpdateMesh();

		bool isSliderHover;
		bool isSliderClick;
		bool hasSliderTexture;
		bool mixSlider;
		bool shouldRemake;
		bool resizeWithBody;
		bool usesSteps;

		glm::vec4 sliderColor;
		glm::vec2 sliderDimensions;
		glm::vec2 sliderSize;
		glm::vec2 lastDimensions;

		float sliderPercentage;
		float minRange;
		float maxRange;
		float padding;
		unsigned int numSteps;
		float stepSize;

		SliderType sType;
		SliderDirection sDir;

		std::shared_ptr<Mesh>sliderMesh;
		std::shared_ptr<Mesh>bodyMesh;
		std::shared_ptr<Texture>sliderTexture;
		std::shared_ptr<Texture>bodyTexture;

		BtnSmartRect rect;

		static inline std::vector<unsigned int> indices = { 0,2,1,2,0,3 };
	};
}