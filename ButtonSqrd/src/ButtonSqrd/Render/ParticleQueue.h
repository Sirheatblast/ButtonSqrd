#pragma once
#include"ButtonSqrd/Core/BtnTime.h"

#include<vector>
#include<set>

namespace BtnSqd {
	class ParticleQueue {
	public:
		ParticleQueue(unsigned int maxParticles);

		bool DispatchBlock(unsigned int pCount, float maxLifeTime, unsigned int& startIdx);
		void Update();


	private:
		struct ParticleBlock {
			unsigned int startIndex;
			unsigned int endIndex;
			float lifeTime;
		};
		struct FreeBlock {
			bool operator<(const FreeBlock& other) const {
				return startIndex < other.startIndex;
			}

			mutable unsigned int startIndex;
			mutable unsigned int endIndex;
		};

		unsigned int maxParticles;
		std::vector<ParticleBlock>blocks;
		std::set<FreeBlock>freeBlocks;
	};
}