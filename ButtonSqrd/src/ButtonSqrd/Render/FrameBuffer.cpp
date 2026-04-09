#include"FrameBuffer.h"
#include"ButtonSqrd/Platform/OpenGL/OpenGLFrameBuffer.h"
#include"RendererApi.h"

namespace BtnSqd {
	FrameBuffer* FrameBuffer::Create(FrameBufferAttatchment frameAttatchment,unsigned int width,unsigned int height){
		switch (RendererApi::GetApi()) {
			case RendererApi::API::NONE:
				return nullptr;
			break;
			case RendererApi::API::OPENGL:
				return new OpenGLFrameBuffer(frameAttatchment,width,height);
		}
		return nullptr;
	}
}