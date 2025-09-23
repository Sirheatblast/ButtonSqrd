#pragma once
#include<vector>
#include<map>

namespace BtnSqd {
	struct AniBone {
		std::string name;
		glm::mat4 finalTransform;
		int parentIndex=-1;
		std::vector<int>children;
	};

	class BtnArmature {
		static glm::mat4 AssimpToGLM(const aiMatrix4x4& m) {
			return glm::mat4(
				m.a1, m.b1, m.c1, m.d1,
				m.a2, m.b2, m.c2, m.d2,
				m.a3, m.b3, m.c3, m.d3,
				m.a4, m.b4, m.c4, m.d4
			);
		}
	public:
		BtnArmature(){}
		BtnArmature(std::unordered_map<std::string, int> boneIds) {
			boneNameToId = boneIds;
		}
		static void Generate(aiNode* node,int parentId,BtnArmature& armature,const glm::mat4& parentTransform) {
			std::string name = node->mName.C_Str();
			glm::mat4 nodeTransform = AssimpToGLM(node->mTransformation);
			glm::mat4 globalTransform = parentTransform*nodeTransform;

			if (armature.boneNameToId.find(name)!= armature.boneNameToId.end()) {
				AniBone bone;
				bone.name = name;
				unsigned int boneId = armature.boneNameToId.at(name);
				bone.finalTransform = globalTransform;
				bone.parentIndex = parentId;
				int temp = 0;


				if (parentId >= 0) {
					armature.bones[parentId].children.push_back(boneId);
				}

				armature.bones.push_back(bone);
			}

			for (uint32_t i = 0; i < node->mNumChildren;i++) {
				Generate(node->mChildren[i],parentId, armature,globalTransform);
			}
		}
		std::vector<AniBone>& GetBones() {return bones;}
		int GetBoneId(std::string name) {
			if (!boneNameToId.contains(name)) {
				return -1;
			}
			return boneNameToId[name];
		}
	private:
		std::vector<AniBone>bones;
		std::unordered_map<std::string, int>boneNameToId;
	};
}

