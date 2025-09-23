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
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessArmature(aiNode* node, const aiScene* scene);
		Mesh GenMesh(aiMesh* mesh,const aiScene* scene);
		std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat,const aiScene* scene, aiTextureType type, std::string typeName);

		std::shared_ptr<Shader>baseShader;
		std::vector<Mesh> meshes;
		std::vector<aiMesh*>meshesAssimp;
		std::string directory;

		BtnArmature* armatureHandle=nullptr;
	};
}