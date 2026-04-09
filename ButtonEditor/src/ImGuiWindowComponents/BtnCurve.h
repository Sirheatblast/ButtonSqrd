#pragma once
#include<imgui.h>
#include<ImCurveEdit.h>
#include<glm/glm.hpp>
#include"ButtonSqrd/Render/Texture.h"

#include<algorithm>
#include<memory>
#include<vector>
#include<map>

namespace BtnSqd {
    class BtnCurve : public ImCurveEdit::Delegate {
    public:
        BtnCurve();
        BtnCurve(std::string name,unsigned int idx);
        BtnCurve(std::string name,unsigned int idx,unsigned int curveCount,ImVec2 minSize,ImVec2 maxSize, uint32_t lineColor);
        size_t GetCurveCount() override;
        ImVec2& GetMin() override;
        ImVec2& GetMax() override;
        size_t GetPointCount(size_t curveIndex) override;
        uint32_t GetCurveColor(size_t curveIndex) override;
        ImVec2* GetPoints(size_t curveIndex) override;
        bool IsVisible(size_t curveIndex) override;
        int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value) override;
        void AddPoint(size_t curveIndex, ImVec2 value) override;
        ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const override{ return ImCurveEdit::CurveSmooth; }

        void SetVisibleLayer(size_t curveIndex = -1);
        unsigned int GetId()const { return idx; }
        std::string GetName()const { return name; }
        bool& GetSyncCurves() {return syncCurves; }
        bool& GetSyncCurveIndexFlag(unsigned int selectedCurve) {return syncIndices[selectedCurve]; }

        BtnCurve& operator=(const BtnCurve& other) {
            if (this != &other) {
                this->min = other.min;
                this->max = other.max;
                this->points = other.points;
                this->name = other.name;
                this->idx = other.idx;
            }
            return *this;
        }

        BtnCurve(const BtnCurve& other) {
            *this = other;
        }

    private:
        std::string name="DefaultCurve";
        unsigned int idx=0;
        int visibleCurveIndex = -1;
        
        bool syncCurves = false;
        std::map<unsigned int,bool>syncIndices;

        ImVec2 max;
        ImVec2 min;
        const int lutResolution = 256;
        const int maxPoints = 8;
        std::vector<std::vector<ImVec2>>points;
    };
}