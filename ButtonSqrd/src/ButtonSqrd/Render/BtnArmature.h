#pragma once
#include<vector>
#include<map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<glm/glm.hpp>

namespace BtnSqd {
	struct AniBone {
		std::string name;
		glm::mat4 finalTransform = glm::mat4(0.0f);
		glm::mat4 globalTransform = glm::mat4(0.0f);
		glm::mat4 localTrasform = glm::mat4(1.0f);
		glm::mat4 offsetMat = glm::mat4(1.0f);
	};

	struct ArmNode {
		std::string name = "EmptyRootNode";
		glm::mat4 nodeTransform =glm::mat4(0.0f);
		std::vector<ArmNode>children;
	};

	class BtnArmature {
	public:
		BtnArmature(){}
		BtnArmature(std::vector<AniBone>boneVec) {
			bones = boneVec;
		}
		
		static ArmNode Generate(aiNode* node,const aiScene* scene, BtnArmature& armature);
		static void UpdateBoneTransforms(ArmNode& node, const glm::mat4& parentTransform,BtnArmature& armature);
		void SetInvserseTransform(glm::mat4 inverse) { inverseTransform = inverse; }
		glm::mat4 GetInverseTransform() { return inverseTransform; }

		std::vector<AniBone>& GetBones() {return bones;}
		std::vector<int>& GetRootBoneId() { return rootBoneIds; }

		const std::unordered_map<std::string, unsigned int>& GetBoneIds() { return nameToBoneId; }

		ArmNode& GetRootNode() { return rootNode; }
		void SetRootNode(ArmNode root) { rootNode = root; }
		void SetBoneIds(std::unordered_map<std::string, unsigned int>& nameToId) {nameToBoneId = nameToId;}
	private:

		std::vector<int> rootBoneIds;
		std::vector<AniBone>bones;
		std::unordered_map<std::string, unsigned int>nameToBoneId;
		ArmNode rootNode;
		glm::mat4 inverseTransform = glm::mat4();
	};
}

