#pragma once

#include"ButtonSqrd/Render/Mesh.h"
#include"ButtonSqrd/Render/Texture.h"
#include"ButtonSqrd/Render/Buffers.h"
#include"ButtonSqrd/Render/Shader.h"
#include"BtnArmature.h"
#include"ButtonSqrd/Core/BtnTime.h"

#include<vector>
#include<string>
#include<filesystem>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace BtnSqd {
	class Model {
	public:
		Model(std::string path="", Shader* shader=nullptr);
		Model(const Model& other);
		void LoadAssimp(std::string& path);
		std::vector<Mesh>* GetMeshes();
		std::shared_ptr<Shader> GetShader() { return baseShader; };
		std::string GetModelName();

		inline std::string GetDirectory()const { return directory; }

	private:
        struct VertexBoneData {
            glm::ivec4 boneIds = glm::ivec4(-1);
            glm::vec4 boneWeights = glm::vec4(0.0f);

            bool AddBoneData(int boneId, float weight) {
                const float EPS = 1e-6f;
                if (weight <= EPS) return false;

                for (int i = 0; i < 4; ++i) {
                    if (boneIds[i] == boneId) {
                        boneWeights[i] += weight;
                        return true;
                    }
                }

                for (int i = 0; i < 4; ++i) {
                    if (boneWeights[i] <= EPS) {
                        boneIds[i] = boneId;
                        boneWeights[i] = weight;
                        return true;
                    }
                }

                int minIndex = 0;
                for (int i = 1; i < 4; ++i) {
                    if (boneWeights[i] < boneWeights[minIndex]) minIndex = i;
                }
                if (weight > boneWeights[minIndex] + EPS) {
                    boneIds[minIndex] = boneId;
                    boneWeights[minIndex] = weight;
                    return true;
                }
                return false;
            }

            void NormalizeAndClamp() {
                const float EPS = 1e-6f;
                float sum = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
                if (sum > EPS) boneWeights /= sum;
                else boneWeights = glm::vec4(0.0f);
                for (int i = 0; i < 4; ++i) {
                    if (boneWeights[i] <= EPS) boneIds[i] = -1;
                }
            }
        };

		struct ArmaturePacket {
			std::unordered_map<std::string,unsigned int>nameToBoneId;
			std::vector<int> meshBase;
			std::vector<VertexBoneData> vertToBone;
		};

		void ProcessNode(aiNode* node, const aiScene* scene, const ArmaturePacket& armPack);
		void ProcessArmature(aiNode* node, const aiScene* scene,ArmaturePacket& armPack);
		void ProcessAnimations(aiNode*node,const aiScene* scene,const ArmaturePacket& armPack);
		void GetRawBones(aiNode* node, const aiScene* scene, ArmaturePacket& armPack, std::unordered_map<std::string, aiBone*>&rawBones);
		
		Mesh GenMesh(aiMesh* mesh,const aiScene* scene, const ArmaturePacket& armPack,unsigned int meshIndex);
		std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat,const aiScene* scene, aiTextureType type, std::string typeName);

		std::shared_ptr<Shader>baseShader;
		std::vector<Mesh> meshes;
		std::vector<aiMesh*>meshesAssimp;
		std::string directory;

	};
}