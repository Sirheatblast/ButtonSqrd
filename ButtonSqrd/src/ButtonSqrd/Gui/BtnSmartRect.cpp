#include "BtnSmartRect.h"

namespace BtnSqd {
	BtnSmartRect::BtnSmartRect(unsigned int texWidth, unsigned int texHeight):width(texWidth),height(texHeight) {
		SetSlicePoints(slicePercentages);
	}

	void BtnSmartRect::UpdateDimensions(unsigned int nWidth, unsigned int nHeight) {
		width = nWidth;
		height = nHeight;
		SetSlicePoints(slicePercentages);
	}

	void BtnSmartRect::SetSlicePoints(RectSlicePercentages nSlicePercentages) {
		slicePercentages = nSlicePercentages;
		slicePoints.sliceUL = slicePercentages.verticalLeft * width;
		slicePoints.sliceUR = (1.0f - slicePercentages.verticalRight) * width;
		slicePoints.sliceLL = slicePercentages.horizUp * height;
		slicePoints.sliceLR = (1.0f - slicePercentages.horizDown) * height;
	}
}
