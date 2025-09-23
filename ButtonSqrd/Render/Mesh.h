#pragma once
#include"ButtonSqrd/Render/Buffers.h"
#include"ButtonSqrd/Render/VertexArray.h"
#include"ButtonSqrd/Render/Shader.h"
#include"ButtonSqrd/Render/Texture.h"
#include"ButtonSqrd/Core/Logger.h"

#include<string>
#include<vector>
#include<memory>
#include<glm/glm.hpp>

#include<assimp/scene.h>

namespace BtnSqd {
	struct Material {
		Material():metallic(0.0f), roughness(1.0f), reflectance(0.0f),emmissiveColor(1.0f),clearColor(1.0f), albedo(nullptr), normal(nullptr) {}

		float metallic;
		float roughness;
		float reflectance;

		glm::vec4 emmissiveColor;
		glm::vec3 clearColor;

		std::shared_ptr<Texture> albedo;
		std::shared_ptr<Texture> normal;
	};
	class Mesh {
	public:
		Mesh(std::vector<Vertices> verts, std::vector<unsigned int>indices,Material mat, BufferLayout layout):verts(verts),indices(indices),material(mat) {
			SetUpMesh(layout);
		}
		
		Mesh(std::vector<Vertices> verts, std::vector<unsigned int>indices, Material mat) :verts(verts), indices(indices), material(mat) {
			SetUpMesh(defaultLayout);
		}

		std::shared_ptr<VertexArray> GetVertexArray()const { return vao; }
		Material* GetMaterial() { return &material; }

		void SetTexture(std::shared_ptr<Texture> nTexture,TextureSlot slot = TextureSlot::Albedo) {
			nTexture->Bind();
			material.albedo = nTexture;
			nTexture->UnBind();
		}

		std::vector<Vertices> GetVerticies()const { return verts; }
		std::vector<unsigned int> GetIndices()const { return indices; }

		void Draw(Shader* shader);
		void Draw();
	private:
		void SetUpMesh(BufferLayout layout);

		std::vector<Vertices> verts;
		std::vector<unsigned int> indices;

		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;
		std::shared_ptr<VertexArray> vao;

		Material material;
		std::map<std::string, unsigned int>boneIds;

		BufferLayout defaultLayout = { 
			{ShaderDataType::Float3,"aPos"},
			{ShaderDataType::Float2, "aTexCoords" },
			{ShaderDataType::Float3, "aNormal" },
			{ShaderDataType::Float3,"aTangentCoords" },
			{ShaderDataType::Float3,"aBitTangentCoords" },
			{ShaderDataType::IntArray,"aBoneIds"},
			{ShaderDataType::FloatArray,"aBoneWeights"},
			{ShaderDataType::Int,"aBoneCount"},
		};
	};
}