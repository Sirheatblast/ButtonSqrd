#include"Model.h"
#include"ButtonSqrd/Core/ResourceManager.h"

namespace BtnSqd {
	Model::Model(std::string path,Shader* shader){
		if (path==""||shader==nullptr) {
			return;
		}
		baseShader.reset(shader);
		baseShader->SetInt("brightness", 100);
		LoadAssimp(path);
		ResourceManager::AddModel(path,*this);
	}

	Model::Model(const Model& other):meshes(other.meshes),baseShader(other.baseShader),directory(other.directory), meshesAssimp(other.meshesAssimp){}

	void Model::LoadAssimp(std::string& path){
		//Load mesh using the Assimp library
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace|aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			BTN_CORE_LOG_TRACE("Error::Assimp: {}", importer.GetErrorString());
		}

		directory = path;
		if (scene) {
			ProcessArmature(scene->mRootNode, scene);
			ProcessNode(scene->mRootNode, scene);
		}
	}

	std::vector<Mesh>* Model::GetMeshes(){
		return &meshes;
	}

	std::string Model::GetModelName() {
		std::filesystem::path p = directory;
		return p.filename().string();
	}

	void Model::ProcessNode(aiNode* node,const aiScene* scene){
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(GenMesh(mesh,scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
	}

	glm::mat4 AssimpToGLM(const aiMatrix4x4& m) {
		return glm::mat4(
			m.a1, m.b1, m.c1, m.d1,
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4
		);
	}
	void AddBoneDataToVertex(Vertices& vertex, int boneID, float weight) {
		for (unsigned int i = 0; i < MAX_BONES_PER_VERTEX; i++) {
			if (vertex.vertexWeights[i] == 0.0f) {
				vertex.boneIds[i] = boneID;
				vertex.vertexWeights[i] = weight;
				vertex.boneCount++;
				return;
			}
		}
	}


	void Model::ProcessArmature(aiNode* node, const aiScene* scene)	{
		if (!scene->HasAnimations())
			return;
		std::unordered_map<std::string, int>boneIds;
		for (unsigned int i = 0; i < scene->mNumMeshes;i++) {
			aiMesh* mesh = scene->mMeshes[i];
			for (unsigned int j = 0; j < mesh->mNumBones; j++) {
				aiBone* bone = mesh->mBones[j];
				std::string name = bone->mName.C_Str();
				boneIds[name] = j;
			}
		}
		BtnArmature armature(boneIds);
		BtnArmature::Generate(node, -1, armature, glm::mat4(1.0f));
		ResourceManager::AddArmature(directory,armature);
		armatureHandle = &ResourceManager::GetLoadedArmatures()[directory];
	}
	Mesh Model::GenMesh(aiMesh* mesh,const aiScene* scene){
		std::vector<Vertices> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			//process vertex positions, normals and texture coordinates
			Vertices vertex;
			glm::vec3 vector;
			//positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			//normals
			if (mesh->HasNormals()) {
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
			}
			//texture coords
			if (mesh->mTextureCoords[0]) {
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
				if (mesh->mTangents) {
					//tangent
					vector.x = mesh->mTangents[i].x;
					vector.y = mesh->mTangents[i].y;
					vector.z = mesh->mTangents[i].z;
					vertex.aTangentCoords = vector;
					//bitangent
					vector.x = mesh->mBitangents[i].x;
					vector.y = mesh->mBitangents[i].y;
					vector.z = mesh->mBitangents[i].z;
					vertex.aBitTangentCoords = vector;
				}
			}
			else
				vertex.texCoords = glm::vec2();
			vertices.push_back(vertex);
		}

		//Indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		//Textures
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		Material meshMat;

		// 1. diffuse maps
		std::vector<Texture*> diffuseMaps = LoadMaterialTextures(mat,scene, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		if(diffuseMaps.size()>0)
			meshMat.albedo.reset(diffuseMaps.back());
		// 2. specular maps
		std::vector<Texture*> specularMaps = LoadMaterialTextures(mat,scene, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture*> normalMaps = LoadMaterialTextures(mat,scene, aiTextureType_HEIGHT, "texture_normal");
		if(normalMaps.size()==0)
			normalMaps = LoadMaterialTextures(mat, scene, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		if(normalMaps.size()>0)
			meshMat.normal.reset(normalMaps.back());
		// 4. height maps
		std::vector<Texture*> heightMaps = LoadMaterialTextures(mat,scene, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		mat->Get(AI_MATKEY_METALLIC_FACTOR, meshMat.metallic);
		mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, meshMat.roughness);
		mat->Get(AI_MATKEY_REFLECTIVITY, meshMat.reflectance);

		if (mesh->HasBones()&&armatureHandle) {
			for (unsigned int i = 0; i < mesh->mNumBones; i++) {
				aiBone* bone = mesh->mBones[i];
				std::string bName = bone->mName.C_Str();
				int boneId = armatureHandle->GetBoneId(bName);

				for (unsigned int j = 0; j < bone->mNumWeights; j++) {
					unsigned int vertId = bone->mWeights[j].mVertexId;
					float weight = bone->mWeights[j].mWeight;
					AddBoneDataToVertex(vertices[vertId], boneId, weight);
				}
			}
		}

		return Mesh(vertices, indices, meshMat);
	}
	std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial* mat,const aiScene* scene, aiTextureType type, std::string typeName){
		std::vector<Texture*> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;

			if (ResourceManager::GetLoadedTextures().contains(str.C_Str())) {
				textures.push_back(ResourceManager::GetLoadedTextures()[str.C_Str()].get());
				skip = true;
			}

			if (str.data[0]=='*') {
				const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
				if (embeddedTexture) {
					std::shared_ptr<Texture> texture;
					TextureSettings texSettings;
					texSettings.wrappingMode = TextureWrappingInfo::Clamp;
					texture.reset(Texture::Create(embeddedTexture->mFilename.C_Str(), texSettings));
					texture->SetType(typeName);
					textures.push_back(texture.get());
					ResourceManager::AddTexture(texture);
					skip = true;
				}
			}

			for (auto& [texPath,tex] : ResourceManager::GetLoadedTextures()) {
				if (std::strcmp(tex->GetPath().c_str(), str.C_Str()) == 0) {
					skip = true;
					break;
				}
			}

			if (!skip) {
				
				std::shared_ptr<Texture> texture;
				TextureSettings texSettings;
				texSettings.wrappingMode = TextureWrappingInfo::Clamp;
				texture.reset(Texture::Create(str.C_Str(),texSettings));
				texture->SetType(typeName);
				textures.push_back(texture.get());
				ResourceManager::AddTexture(texture);
			}
		}
		return textures;
	}
}