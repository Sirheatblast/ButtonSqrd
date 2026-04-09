#include"Texture.h"
#include"Renderer.h"

#include"ButtonSqrd/Platform/OpenGL/OpenGLTexture.h"
namespace BtnSqd {
	Texture* Texture::Create(std::string const path,TextureSettings texSettings){
		switch (Renderer::GetApi()) {
		case RendererApi::API::NONE:
			return nullptr;

		case RendererApi::API::OPENGL:
			return new OpenGLTexture(path,texSettings);
			break;
		}
		return nullptr;
	}
	Texture* Texture::Create(unsigned int width, unsigned int height, TextureSettings texSettings,void* data){
		switch (Renderer::GetApi()) {
		case RendererApi::API::NONE:
			return nullptr;

		case RendererApi::API::OPENGL:
			return new OpenGLTexture(width,height, texSettings,data);
			break;
		}
		return nullptr;
	}
}