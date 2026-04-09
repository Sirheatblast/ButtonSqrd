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
		const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			BTN_CORE_LOG_TRACE("Error::Assimp: {}", importer.GetErrorString());
		}

		directory = path;
		if (scene) {
			ArmaturePacket armPack,armC;
			ProcessArmature(scene->mRootNode, scene,armPack);
			ProcessAnimations(scene->mRootNode,scene,armPack);
			ProcessNode(scene->mRootNode, scene,armPack);
		}
	}

	std::vector<Mesh>* Model::GetMeshes(){
		return &meshes;
	}

	std::string Model::GetModelName() {
		std::filesystem::path p = directory;
		return p.filename().string();
	}

	static void NormalizeWeights(Vertices& vertex) {

		std::vector<std::pair<int, float>> influences;
		for (int i = 0; i < 4; ++i) {
			if (vertex.vertexWeights[i] > 0.0f)
				influences.emplace_back(vertex.boneIds[i], vertex.vertexWeights[i]);
		}
		std::sort(influences.begin(), influences.end(), [](auto& a, auto& b) {	return a.second > b.second;});
		while (influences.size() < 4) influences.emplace_back(-1, 0.0f);

		float total = influences[0].second + influences[1].second + influences[2].second + influences[3].second;
		if (total > 0.0f) {
			for (auto& [id, w] : influences) w /= total;
		}

		for (int i = 0; i < 4; ++i) {
			vertex.boneIds[i] = influences[i].first;
			vertex.vertexWeights[i] = influences[i].second;
		}

		float sum = vertex.vertexWeights.x + vertex.vertexWeights.y + vertex.vertexWeights.z + vertex.vertexWeights.w;
		if (sum > 0.0f) {
			vertex.vertexWeights /= sum;
		}		
	}

	void Model::ProcessNode(aiNode* node,const aiScene* scene,const ArmaturePacket& armPack){
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Mesh btnMesh = GenMesh(mesh, scene,armPack,i);
			meshes.push_back(btnMesh);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene,armPack);
		}
	}

	glm::mat4 AssimpToGLM(const aiMatrix4x4& m) {
		glm::mat4 mat(
			m.a1, m.a2, m.a3, m.a4,
			m.b1, m.b2, m.b3, m.b4,
			m.c1, m.c2, m.c3, m.c4,
			m.d1, m.d2, m.d3, m.d4
		);
		return glm::transpose(mat);
	}


	void AddBoneDataToVertex(Vertices& vertex, int boneId, float weight) {
		for (int i = 0; i < 4; ++i) {
			if (vertex.boneIds[i] == boneId) {
				vertex.vertexWeights[i] += weight;
				return;
			}
		}
		for (int i = 0; i < 4; ++i) {
			if (vertex.vertexWeights[i] == 0.0f) {
				vertex.boneIds[i] = boneId;
				vertex.vertexWeights[i] = weight;
				return;
			}
		}

		int minIndex = 0;
		for (int i = 1; i < 4; ++i) {
			if (vertex.vertexWeights[i] < vertex.vertexWeights[minIndex]) minIndex = i;
		}
		if (weight > vertex.vertexWeights[minIndex]) {
			vertex.boneIds[minIndex] = boneId;
			vertex.vertexWeights[minIndex] = weight;
		}

	}

	void Model::GetRawBones (aiNode* node, const aiScene* scene, ArmaturePacket& armPack, std::unordered_map<std::string, aiBone*>&rawBones) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			armPack.meshBase[i] = mesh->mNumVertices;
			for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
				armPack.vertToBone.push_back(VertexBoneData());
			}
			for (unsigned int j = 0; j < mesh->mNumBones; j++) {
				std::string bName = mesh->mBones[j]->mName.C_Str();
				if (!armPack.nameToBoneId.contains(bName)) {
					armPack.nameToBoneId[bName] = armPack.nameToBoneId.size();
				}
				rawBones.insert({ bName,mesh->mBones[j] });

				for (unsigned int k = 0; k < rawBones[bName]->mNumWeights; k++) {
					auto rawWeight = rawBones[bName]->mWeights[k];
					unsigned int baseOffset =(i>0)?armPack.meshBase[i-1]:0;
					armPack.vertToBone[baseOffset + rawWeight.mVertexId].AddBoneData(armPack.nameToBoneId[bName], rawWeight.mWeight);
				}
			}	

		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			GetRawBones(node->mChildren[i], scene, armPack,rawBones);
		}
	}


	void Model::ProcessArmature(aiNode* node, const aiScene* scene, ArmaturePacket& armPack)	{
		//Needs to fill in the armature packet with data and create a mapping for global mesh bone id's
		//Also needs to fill the mesh base vector in the pack that hold the number of vertices per mesh
		//Also fill out the vertToBone vector that holds the vertex weight information
		if (!scene->HasAnimations())
			return;

		armPack.meshBase.resize(scene->mNumMeshes);
		std::unordered_map<std::string, aiBone*>rawBones;
		GetRawBones(scene->mRootNode, scene, armPack, rawBones);

		std::vector<AniBone> bones;
		bones.resize(armPack.nameToBoneId.size());

		for (auto& [name, id] : armPack.nameToBoneId) {
			aiBone* rawBone = rawBones[name];
			AniBone bone;
			bone.name = name;
			bone.offsetMat = AssimpToGLM(rawBone->mOffsetMatrix);
			bones[id] = bone;
		}

		BtnArmature armature(bones);
		armature.SetBoneIds(armPack.nameToBoneId);

		ArmNode rootNode = BtnArmature::Generate(scene->mRootNode, scene, armature);
		armature.SetRootNode(rootNode);
		armature.SetInvserseTransform(glm::inverse(rootNode.nodeTransform));
		ResourceManager::AddArmature(directory, armature);
	}

	void Model::ProcessAnimations(aiNode* node,const aiScene* scene,const ArmaturePacket& armPack)	{
		if (armPack.nameToBoneId.empty()) {
			return;
		}

		for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
			aiAnimation* rawAnimation = scene->mAnimations[i];
			std::string name = rawAnimation->mName.C_Str();
			BtnAnimation animation(name);
			animation.SetDuration(rawAnimation->mDuration);
			animation.SetTicksPerSecond(rawAnimation->mTicksPerSecond);
			
			for (unsigned int j = 0; j < rawAnimation->mNumChannels;j++) {
				auto* channel = rawAnimation->mChannels[j];
				std::string boneName = channel->mNodeName.C_Str();
				unsigned int maxFrames =(channel->mNumPositionKeys>channel->mNumRotationKeys)?channel->mNumPositionKeys:channel->mNumRotationKeys;
				if (maxFrames < channel->mNumScalingKeys) {
					maxFrames = channel->mNumScalingKeys;
				}
				for (unsigned int k = 0; k < maxFrames;k++) {
					KeyFrame frame;
					frame.index = k;

					float frameTime = (channel->mPositionKeys[k].mTime > channel->mRotationKeys[k].mTime) ? channel->mPositionKeys[k].mTime : channel->mRotationKeys[k].mTime;
					if (frameTime < channel->mScalingKeys[k].mTime) {
						frameTime = channel->mScalingKeys[k].mTime;
					}
					frame.time = frameTime;

					if (k< channel->mNumPositionKeys) {
						auto pos = channel->mPositionKeys[k].mValue;
						frame.position = glm::vec3(pos.x,pos.y,pos.z);
					}
					if (k < channel->mNumRotationKeys) {
						auto rot = channel->mRotationKeys[k].mValue;
						frame.rotation = glm::quat(rot.w, rot.x, rot.y, rot.z);
					}
					if (k < channel->mNumScalingKeys) {
						auto scale = channel->mScalingKeys[k].mValue;
						frame.scale = glm::vec3(scale.x, scale.y, scale.z);
					}
					animation.AddKeyFrame(boneName,frame);
				}
			}

			ResourceManager::AddAnimation(name, animation);
		}
	}

	Mesh Model::GenMesh(aiMesh* mesh,const aiScene* scene, const ArmaturePacket& armPack,unsigned int meshIndex){
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

		//Add vertex weights
		if (!armPack.vertToBone.empty()) {
			unsigned int vertOffset = (meshIndex > 0) ? armPack.meshBase[meshIndex - 1] : 0;
			for (unsigned int j = 0; j < vertices.size(); j++) {
				glm::ivec4 boneIds = armPack.vertToBone[vertOffset + j].boneIds;
				glm::vec4 boneWeights = armPack.vertToBone[vertOffset + j].boneWeights;
				vertices[j].boneIds = boneIds;
				vertices[j].vertexWeights = boneWeights;
				NormalizeWeights(vertices[j]);
			}
		}

		mat->Get(AI_MATKEY_METALLIC_FACTOR, meshMat.metallic);
		mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, meshMat.roughness);
		mat->Get(AI_MATKEY_REFLECTIVITY, meshMat.reflectance);
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