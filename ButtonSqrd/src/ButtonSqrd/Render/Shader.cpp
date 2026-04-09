#include "Shader.h"

#include"ButtonSqrd/Platform/OpenGL/OpenGLShader.h"
#include"Renderer.h"

namespace BtnSqd {
	Shader* Shader::Create(const std::string vertexSource, const std::string fragmentSource){
		switch (Renderer::GetApi()) {
			case RendererApi::API::NONE: return nullptr;
			case RendererApi::API::OPENGL: return new OpenGLShader(vertexSource, fragmentSource);
		}
		return nullptr;
	}
	Shader* Shader::Create(const std::string vertexSource, const std::string geoSource, const std::string fragmentSource){
		switch (Renderer::GetApi()) {
			case RendererApi::API::NONE: return nullptr;
			case RendererApi::API::OPENGL: return new OpenGLShader(vertexSource, geoSource,fragmentSource);
		}
		return nullptr;
	}
	Shader* Shader::Create(const std::string& computeSource) {
		switch (Renderer::GetApi()) {
		case RendererApi::API::NONE: return nullptr;
		case RendererApi::API::OPENGL: return new OpenGLShader(computeSource);
		}
		return nullptr;
	}
}
