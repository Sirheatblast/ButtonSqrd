#pragma once

#include<glm/glm.hpp>
#include<ButtonSqrd/Render/RenderUtils.h>

namespace BtnSqd {
	struct RectSlicePoints {
		float sliceUL = 0.0f;
		float sliceUR = 0.0f;
		float sliceLL = 0.0f;
		float sliceLR = 0.0f;
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