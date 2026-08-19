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

		const glm::vec4 GetSliderFinalColor();

		const glm::vec4 GetSliderColor() { return sliderColor; }
		void SetSliderColor(glm::vec4 nColor) { sliderColor = nColor; }

		const glm::vec4 GetSliderHoverColor() { return sliderHoverColor; }
		void SetSliderHoverColor(glm::vec4 nColor) { sliderHoverColor = nColor; }

		const glm::vec4 GetSliderClickColor() { return sliderClickColor; }
		void SetSliderClickColor(glm::vec4 nColor) { sliderClickColor = nColor; }

		void SetSliderPercentage(float percent);
		const float GetSliderPercentage() { return sliderPercentage; }

		const bool GetHasBodyTexture() { return hasBodyTexture; }
		void SetHasBodyTexture(bool hasTex) { hasBodyTexture = hasTex; }
		void SetBodyTexture(std::string texturePath);

		const bool GetHasSliderTexture() { return hasSliderTexture; }
		void SetHasSliderTexture(bool hasTex) { hasSliderClickTexture = hasTex; }
		void SetSliderTexture(std::string texturePath);

		const bool GetHasSliderHoverTexture() { return hasSliderHoverTexture; }
		void SetHasSliderHoverTexture(bool hasTex) { hasSliderHoverTexture = hasTex; }
		void SetSliderHoverTexture(std::string texturePath);

		const bool GetHasSliderClickTexture() { return hasSliderHoverTexture; }
		void SetHasSliderClickTexture(bool hasTex) { hasSliderClickTexture = hasTex; }
		void SetSliderClickTexture(std::string texturePath);

		const bool GetMixSlider();
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

		const float GetSliderValue();

		const bool GetSliderHover() { return isSliderHover; }
		void SetSliderHover(bool hover) { isSliderHover = hover; }

		const bool GetSliderClick() { return isSliderClick; }
		void SetSliderClick(bool click) { isSliderClick = click; }

		const bool GetJumpToClick() { return jumpToClick; }
		void SetJumpToClick(bool shouldJump) { jumpToClick = shouldJump; }

		const bool GetUseSliderHoverColor() { return useSliderHoverColor; }
		void SetUseSliderHoverColor(bool use) { useSliderHoverColor = use; }

		const bool GetUseSliderClickColor() { return useSliderClickColor; }
		void SetUseSliderClickColor(bool use) { useSliderClickColor = use; }

		const bool GetUseSliderHoverTexture() { return useSliderHoverTexture; }
		void SetUseSliderHoverTexture(bool use) { useSliderHoverTexture = use; }

		const bool GetUseSliderClickTexture() { return useSliderClickTexture; }
		void SetUseSliderClickTexture(bool use) { useSliderClickTexture = use; }

		const bool GetSliderNineSlice() { return sliderNineSlice; }
		void SetSliderNineSlice(bool slice) { sliderNineSlice = slice; }

		const float GetSliderTexScale() { return sliderTexScale; }
		void SetSliderTexScale(float nScale) { sliderTexScale = nScale; }

		std::shared_ptr<Texture> GetSliderTexture() { return sliderTexture; }
		std::shared_ptr<Texture> GetSliderHoverTexture() { return sliderHoverTexture; }
		std::shared_ptr<Texture> GetSliderClickTexture() { return sliderClickTexture; }
		std::shared_ptr<Texture> GetBodyTexture() { return bodyTexture; }

	private:
		std::vector<Vertices> GenerateBodyVerts();
		std::vector<Vertices> GenerateSliderVerts();
		void UpdateMesh();

		bool isSliderHover;
		bool isSliderClick;
		bool hasBodyTexture;
		bool hasSliderTexture;
		bool hasSliderHoverTexture;
		bool hasSliderClickTexture;
		bool mixSlider;
		bool shouldRemake;
		bool resizeWithBody;
		bool usesSteps;
		bool jumpToClick;
		bool useSliderHoverColor;
		bool useSliderClickColor;
		bool useSliderHoverTexture;
		bool useSliderClickTexture;
		bool sliderNineSlice;

		glm::vec4 sliderColor;
		glm::vec4 sliderHoverColor;
		glm::vec4 sliderClickColor;

		glm::vec2 sliderDimensions;
		glm::vec2 sliderSize;
		glm::vec2 lastDimensions;

		float sliderPercentage;
		float minRange;
		float maxRange;
		float padding;
		unsigned int numSteps;
		float stepSize;
		float sliderTexScale;

		SliderType sType;
		SliderDirection sDir;

		std::shared_ptr<Mesh>sliderMesh;
		std::shared_ptr<Mesh>bodyMesh;
		std::shared_ptr<Texture>sliderTexture;
		std::shared_ptr<Texture>sliderHoverTexture;
		std::shared_ptr<Texture>sliderClickTexture;
		std::shared_ptr<Texture>bodyTexture;

		BtnSmartRect rect;
		BtnSmartRect sliderRect;

		static inline std::vector<unsigned int> indices = { 0,2,1,2,0,3 };
	};
}