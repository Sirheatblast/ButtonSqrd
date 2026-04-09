#include "BtnArmature.h"
#include"ButtonSqrd/Core/Logger.h"

glm::mat4 AssimpToGLM(const aiMatrix4x4& m) {
    glm::mat4 mat(
        m.a1, m.a2, m.a3, m.a4,
        m.b1, m.b2, m.b3, m.b4,
        m.c1, m.c2, m.c3, m.c4,
        m.d1, m.d2, m.d3, m.d4
    );
    return glm::transpose(mat);
}


namespace BtnSqd {

	ArmNode BtnArmature::Generate(aiNode* node, const aiScene* scene, BtnArmature& armature) {
        ArmNode aNode;
        aNode.name = node->mName.C_Str();
        aNode.nodeTransform = AssimpToGLM(node->mTransformation);
        if (armature.nameToBoneId.contains(aNode.name)) {
            auto& bones = armature.GetBones();
            auto& bone = bones[armature.nameToBoneId.at(aNode.name)];
            bone.localTrasform = aNode.nodeTransform;
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            aNode.children.push_back(Generate(node->mChildren[i],scene, armature));
        }
        return aNode;
    }

	void BtnArmature::UpdateBoneTransforms(ArmNode& node, const glm::mat4& parentTransform, BtnArmature& armature) {
        glm::mat4 globalTransform = parentTransform * node.nodeTransform;
        if (auto it = armature.nameToBoneId.find(node.name); it != armature.nameToBoneId.end()) {
            auto& bones = armature.GetBones();
            auto& bone = bones[it->second];

            globalTransform = parentTransform * bone.localTrasform;
            bone.globalTransform = globalTransform;
            bone.finalTransform = globalTransform * bone.offsetMat;
        }
        for (auto& child:node.children) {
            UpdateBoneTransforms(child, globalTransform, armature);
        }
	}
}
