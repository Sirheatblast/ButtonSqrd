#include"Mesh.h"

namespace BtnSqd {
	void Mesh::Draw(Shader* shader){
		if (material.albedo != nullptr) {
			shader->SetBool("useAlbedoTexture", 1);
			shader->SetUniform("ourTexture",0);
			material.albedo->Bind(TextureSlot::Albedo);
		}
		else {
			shader->SetUniform("useAlbedoTexture", 0);
		}

		if (material.normal != nullptr) {
			shader->SetBool("useNormalMap", 1);
			material.normal->Bind(TextureSlot::Normal);
		}
		else {
			shader->SetUniform("useNormalMap", 0);
		}

		vao->Bind();
	}

	//Only draws a texture with no shading
	void Mesh::Draw() {
		if (material.albedo != nullptr) {
			material.albedo->Bind(TextureSlot::Albedo);
		}
		if (material.normal != nullptr) {
			material.normal->Bind(TextureSlot::Normal);
		}

		vao->Bind();
	}

	void Mesh::SetUpMesh(BufferLayout layout){
		vbo.reset(VertexBuffer::Create(verts.data(), sizeof(verts[0]) * verts.size()));
		ibo.reset(IndexBuffer::Create(this->indices.data(), this->indices.size()));
		vao.reset(VertexArray::Create());
		vao->SetIndexBuffer(ibo);

		vbo->SetLayout(layout);
		vao->AddVertexBuffer(vbo);
	}
}