#include"OpenGLVertexArray.h"

namespace BtnSqd {
	static GLenum ShaderDataTypeConversion(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:return GL_FLOAT;
		case ShaderDataType::Float2:return GL_FLOAT;
		case ShaderDataType::Float3:return GL_FLOAT;
		case ShaderDataType::Float4:return GL_FLOAT;

		case ShaderDataType::Int:return GL_INT;
		case ShaderDataType::Int2:return GL_INT;
		case ShaderDataType::Int3:return GL_INT;
		case ShaderDataType::Int4:return GL_INT;

		case ShaderDataType::Mat3:return GL_FLOAT;
		case ShaderDataType::Mat4:return GL_FLOAT;

		case ShaderDataType::Bool:return GL_BOOL;
		}
		return GLenum();
	}

	OpenGLVertexArray::OpenGLVertexArray(){
		glGenVertexArrays(1, &rendererId);
	}
	void OpenGLVertexArray::Bind() const{
		glBindVertexArray(rendererId);
	}
	void OpenGLVertexArray::Unbind() const{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuffer){
		glBindVertexArray(rendererId);
		vBuffer->Bind();

		uint32_t index = 0;
		for (const auto& element : vBuffer->GetLayout()) {
			glEnableVertexAttribArray(index);
			auto dType = ShaderDataTypeConversion(element.type);
			if (dType !=GL_INT) {
				glVertexAttribPointer(index, element.GetCompCount(), dType, element.normalized ? GL_TRUE : GL_FALSE, vBuffer->GetLayout().GetStride(), (const void*)element.offset);
			}
			else {
				glVertexAttribIPointer(index, 1, dType, vBuffer->GetLayout().GetStride(), (const void*)element.offset);
			}
			++index;
		}

		vertexBufferList.emplace_back(vBuffer);
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuffer){
		glBindVertexArray(rendererId);
		iBuffer->Bind();
		indexBuffer = iBuffer;
	}
	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::getVertexBuffers(){
		return vertexBufferList;
	}
	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::getIndexBuffer(){
		return indexBuffer;
	}
}