#include "BtnCurve.h"
#include<ButtonSqrd/Core/Logger.h>


namespace BtnSqd {
	BtnCurve::BtnCurve() {
		min = ImVec2(0.0f, 0.0f);
		max = ImVec2(150.0f, 100.0f);
		points.push_back(std::vector<ImVec2>());
		syncIndices.insert({ 0,false });
	}
	BtnCurve::BtnCurve(std::string name, unsigned int idx) :name(name), idx(idx) {
		min = ImVec2(0.0f, 0.0f);
		max = ImVec2(150.0f, 100.0f);
		points.push_back(std::vector<ImVec2>());
		syncIndices.insert({ 0,false });
	}
	BtnSqd::BtnCurve::BtnCurve(std::string name, unsigned int idx, unsigned int curveCount, ImVec2 minSize, ImVec2 maxSize, uint32_t lineColor) :name(name), max(maxSize), min(minSize), idx(idx) {
		for (int i = 0; i < curveCount; i++) {
			points.push_back(std::vector<ImVec2>());
			syncIndices.insert({ i,false });
		}
	}

	size_t BtnSqd::BtnCurve::GetCurveCount() {
		return points.size();
	}

	ImVec2& BtnSqd::BtnCurve::GetMin() {
		return min;
	}

	ImVec2& BtnSqd::BtnCurve::GetMax() {
		return max;
	}

	size_t BtnSqd::BtnCurve::GetPointCount(size_t curveIndex) {
		return points[curveIndex].size();
	}

	uint32_t BtnSqd::BtnCurve::GetCurveColor(size_t curveIndex) {
		static const uint32_t colors[] = {
			ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)),
			ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)),
			ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 1.0f))
		};

		return colors[curveIndex % 3];
	}

	ImVec2* BtnSqd::BtnCurve::GetPoints(size_t curveIndex) {
		return points[curveIndex].data();
	}

	bool BtnCurve::IsVisible(size_t curveIndex) {
		return (visibleCurveIndex == -1) || (curveIndex == (size_t)visibleCurveIndex);
	}

	int BtnSqd::BtnCurve::EditPoint(size_t curveIndex, int pointIndex, ImVec2 value) {
		points[curveIndex][pointIndex] = value;
		int editedPoint = pointIndex;
		std::sort(points[curveIndex].begin(), points[curveIndex].end(), [](ImVec2 a, ImVec2 b) { return a.x < b.x; });
		for (size_t i = 0; i < GetPointCount(curveIndex); i++) {
			if (points[curveIndex][i].x == value.x) {
				for (ImVec2& p : points[curveIndex]) {
					p.x = glm::clamp(p.x, min.x, max.x);
					p.y = glm::clamp(p.y, min.y, max.y);
				}
				points[curveIndex].front() = ImVec2(min.x, points[curveIndex].front().y);
				points[curveIndex].back() = ImVec2(max.x, points[curveIndex].back().y);
				editedPoint = (int)i;
			}
		}
		if (syncCurves && syncIndices[curveIndex]) {
			for (auto [targetIdx, doesSync] : syncIndices) {
				if (!doesSync || targetIdx == curveIndex) {
					continue;
				}
				points[targetIdx] = points[curveIndex];
			}
		}

		return editedPoint;
	}

	void BtnSqd::BtnCurve::AddPoint(size_t curveIndex, ImVec2 value) {
		if (points[curveIndex].size() > maxPoints) {
			return;
		}

		points[curveIndex].push_back(value);
		std::sort(points[curveIndex].begin(), points[curveIndex].end(), [](ImVec2 a, ImVec2 b) { return a.x < b.x; });
	}
	void BtnCurve::SetVisibleLayer(size_t curveIndex) {
		visibleCurveIndex = (int)curveIndex;
	}
}