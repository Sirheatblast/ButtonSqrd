#pragma once
#include"ButtonSqrd/Render/Buffers.h"
#include"ButtonSqrd/Render/Renderer.h"

#include<glad/glad.h>

namespace BtnSqd {

	class OpenGLVertexBuffer:public VertexBuffer {
	public:
		OpenGLVertexBuffer(Vertices* vertices, uint32_t size);
		~OpenGLVertexBuffer() override;
		void Bind() const override;
		void Unbind() const override;
		void SetLayout(BufferLayout& layout) override;
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