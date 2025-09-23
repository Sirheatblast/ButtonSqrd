#include "Buffers.h"
#include"Renderer.h"

#include"ButtonSqrd/Platform/OpenGL/OpenGLBuffers.h"

namespace BtnSqd {
	VertexBuffer* VertexBuffer::Create(Vertices* vertices, unsigned int size){
		switch (Renderer::GetApi()) {
		case RendererApi::API::NONE: return nullptr;
		case RendererApi::API::OPENGL: return new OpenGLVertexBuffer(vertices,size);
		}

		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(unsigned int* indices, unsigned int count){
		switch (Renderer::GetApi()) {
		case RendererApi::API::NONE: return nullptr;
		case RendererApi::API::OPENGL: return new OpenGLIndexBuffer(indices, count);
		}
		return nullptr;
	}
}