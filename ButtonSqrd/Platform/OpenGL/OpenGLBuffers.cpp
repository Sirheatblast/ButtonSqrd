#include "OpenGLBuffers.h"

namespace BtnSqd {
	//Vertex Buffer
	OpenGLVertexBuffer::OpenGLVertexBuffer(Vertices* vertices, uint32_t size){
		glCreateBuffers(1, &renderId);
		glBindBuffer(GL_ARRAY_BUFFER, renderId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer(){
		glDeleteBuffers(1, &renderId);
	}
	void OpenGLVertexBuffer::Bind() const{
		glBindBuffer(GL_ARRAY_BUFFER, renderId);
	}
	void OpenGLVertexBuffer::Unbind() const{
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}

	void OpenGLVertexBuffer::SetLayout(BufferLayout& layout){
		bufferLayout = layout;
	}

	//Index Buffer
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, uint32_t count):indexCount(count){
		glCreateBuffers(1, &renderId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer(){
		glDeleteBuffers(1, &renderId);
	}
	void OpenGLIndexBuffer::Bind() const{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderId);
	}
	void OpenGLIndexBuffer::Unbind() const{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
