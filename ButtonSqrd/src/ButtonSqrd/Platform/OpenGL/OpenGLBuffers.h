#pragma once
#include"ButtonSqrd/Render/Buffers.h"
#include"ButtonSqrd/Render/Renderer.h"

#include<glad/glad.h>

namespace BtnSqd {
	class OpenGLShaderStorageBuffer:public ShaderStorageBuffer {
	public:
		OpenGLShaderStorageBuffer(unsigned int bindingSlot);
		void Bind()override;

		void AttachData(const void*data,size_t dataSize)override;
		void UpdateData(const void* data, size_t dataSize, size_t offset = 0)override;
	private:
		unsigned int id=0;
		unsigned int binding;
	};

	class OpenGLVertexBuffer:public VertexBuffer {
	public:
		OpenGLVertexBuffer(Vertices* vertices, uint32_t size);
		~OpenGLVertexBuffer() override;
		void Bind() const override;
		void Unbind() const override;
		void SetLayout(BufferLayout& layout) override;
		void UpdateBuffer(void* nData, size_t size, size_t offset) override;
		BufferLayout& GetLayout() override { return bufferLayout; }

	private:
		uint32_t renderId;
		BufferLayout bufferLayout;
	};

	class OpenGLIndexBuffer :public IndexBuffer {
	public:
		OpenGLIndexBuffer(unsigned int* indices, uint32_t count);
		~OpenGLIndexBuffer() override;
		void Bind() const override;
		void Unbind() const override;
		virtual unsigned int GetCount()const { return indexCount; };

	private:
		uint32_t renderId;
		uint32_t indexCount;
	};
}