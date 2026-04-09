#include"RendererCommand.h"

#include"ButtonSqrd/Platform/OpenGL/OpenGLRendererApi.h"
namespace BtnSqd {
	RendererApi* RenderCommand::renApi = new OpenGLRenderApi();
}