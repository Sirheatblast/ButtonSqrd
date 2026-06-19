#pragma once

#include<glm/glm.hpp>
#include<ButtonSqrd/Render/RenderUtils.h>

namespace BtnSqd {
	struct RectSlicePoints {
		glm::vec2 sliceUL = glm::vec2(0.0f);
		glm::vec2 sliceUR = glm::vec2(0.0f);
		glm::vec2 sliceLL = glm::vec2(0.0f);
		glm::vec2 sliceLR = glm::vec2(0.0f);
	};

	class BtnSmartRect {
	public:
		BtnSmartRect(unsigned int texWidth, unsigned int texHeight);
		BtnSmartRect() = default;

		void UpdateDimensions(unsigned int nWidth, unsigned int nHeight);

		void SetSlicePoints(RectSlicePercentages slicePercentages);
		RectSlicePoints GetSlicePoints()const { return slicePoints; }
		RectSlicePercentages GetSlicePercentages()const { return slicePercentages; }
	private:
		RectSlicePoints slicePoints;
		RectSlicePercentages slicePercentages;
		unsigned int width = 0;
		unsigned int height = 0;
	};
}