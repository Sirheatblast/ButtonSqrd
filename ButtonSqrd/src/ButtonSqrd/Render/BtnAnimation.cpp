#include "BtnAnimation.h"
namespace BtnSqd {
    void BtnAnimation::AddKeyFrame(std::string boneName, KeyFrame newFrame) {
        if (boneKeyFrames.find(boneName)!=boneKeyFrames.end()) {
            boneKeyFrames.at(boneName).push_back(newFrame);
        }
        else {
            boneKeyFrames.insert({ boneName, std::vector<KeyFrame>() });
            boneKeyFrames.at(boneName).push_back(newFrame);
        }
    }
    std::vector<KeyFrame> BtnAnimation::GetKeyFrames(std::string boneName) const {
        auto it = boneKeyFrames.find(boneName);
        if (it != boneKeyFrames.end()) {
            return boneKeyFrames.at(boneName);
        }
        return std::vector<KeyFrame>();
    }
}

