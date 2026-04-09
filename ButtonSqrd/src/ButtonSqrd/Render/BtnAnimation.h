#pragma once
#include<string>
#include<vector>
#include<map>

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

namespace BtnSqd {

	struct KeyFrame {
		float time;
		unsigned int index;
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	};

	class BtnAnimation {
	public:
		BtnAnimation(){}
		BtnAnimation(std::string name,float ticksPsec=0.0f,float animDuration=0.0f) {
			animName = name;
			ticksPerSecond = ticksPsec;
			duration = animDuration;
		}

		void AddKeyFrame(std::string boneName, KeyFrame newFrame);
		std::vector<KeyFrame> GetKeyFrames(std::string boneName)const;
		std::map<std::string, std::vector<KeyFrame>> GetBoneKeyFrames()const {
			return boneKeyFrames;
		}
		std::string GetName()const { return animName; }
		float GetTicksPerSecond()const { return ticksPerSecond; }
		float GetDuration()const { return duration; }

		void SetTicksPerSecond(float tps) { ticksPerSecond = tps; }
		void SetDuration(float dur) { duration = dur; }
	private:
		std::string animName;
		float ticksPerSecond=0.0f;
		float duration = 0.0f;
		std::map<std::string,std::vector<KeyFrame>>boneKeyFrames;
	};

	struct AnimationConnection {
		std::string toAnim;
		int ticks;
	};

	struct AnimationPack {
		BtnAnimation animation;
		float playbackSpeed = 1.0f;
		bool isPlaying = false;
		bool isLooping = false;
		bool isDefault = false;
		std::map<std::string, AnimationConnection> connections;
		glm::vec2 nodePos = glm::vec2(0.0f);

		bool operator==(const AnimationPack& other) const {
			return animation.GetName() == other.animation.GetName();
		}

	};
}