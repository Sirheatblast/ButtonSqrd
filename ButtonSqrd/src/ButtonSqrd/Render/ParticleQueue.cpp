#include "ParticleQueue.h"
#include"ButtonSqrd/Core/Logger.h"
namespace BtnSqd{
    ParticleQueue::ParticleQueue(unsigned int maxParticles):maxParticles(maxParticles) {
        freeBlocks.insert(FreeBlock({0,maxParticles-1}));
    }

    bool ParticleQueue::DispatchBlock(unsigned int pCount,float maxLifeTime, unsigned int& startIdx) {
        for (auto& freeB:freeBlocks) {
            unsigned int netSpace = freeB.endIndex - freeB.startIndex;
            if (netSpace > pCount) {
                startIdx = freeB.startIndex;
                freeB.startIndex = freeB.startIndex + pCount;
                blocks.push_back(ParticleBlock({ startIdx ,(startIdx + pCount)-1,maxLifeTime }));
                return true;
            }
        }
        return false;
    }

    void ParticleQueue::Update() {
        float dt = BtnTime::GetDeltaTime();

        for (auto block = blocks.begin(); block != blocks.end();) {
            block->lifeTime -= dt;
            if (block->lifeTime <= 0.0f) {
                FreeBlock nFblock = { block->startIndex,block->endIndex };
                freeBlocks.insert(nFblock);
                block = blocks.erase(block);
            }
            else {
                block++;
            }
        }

        if (freeBlocks.size() > 1) {
            std::vector<FreeBlock> merged;
            auto it = freeBlocks.begin();

            FreeBlock current = *it;
            ++it;

            while (it != freeBlocks.end()) {
                if (current.endIndex+1 == it->startIndex) {
                    current.endIndex = it->endIndex;
                }
                else {
                    merged.push_back(current);
                    current = *it;
                }
                ++it;
            }
            merged.push_back(current);
            freeBlocks.clear();
            freeBlocks.insert(merged.begin(), merged.end());
        }
    }
}
