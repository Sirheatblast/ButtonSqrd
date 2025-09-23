#include"VertexArray.h"
#include"Renderer.h"

#include"ButtonSqrd/Platform/OpenGL/OpenGLVertexArray.h"
namespace BtnSqd {
	VertexArray* VertexArray::Create(){
		switch (Renderer::GetApi()) {
		case RendererApi::API::NONE: return nullptr;
		case RendererApi::API::OPENGL: return new OpenGLVertexArray();
		}
	}
}