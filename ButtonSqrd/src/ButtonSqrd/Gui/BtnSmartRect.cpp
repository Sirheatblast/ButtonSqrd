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
		slicePoints.sliceUL = glm::vec2(width * slicePercentages.verticalLeft, height * slicePercentages.horizUp);
		slicePoints.sliceUR = glm::vec2(width * slicePercentages.verticalRight, height * slicePercentages.horizUp);
		slicePoints.sliceLL = glm::vec2(width * slicePercentages.verticalLeft, height * slicePercentages.horizDown);
		slicePoints.sliceLR = glm::vec2(width * slicePercentages.verticalRight, height * slicePercentages.horizDown);
	}
}
