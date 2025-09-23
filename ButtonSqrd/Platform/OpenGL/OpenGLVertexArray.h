#pragma once
#include"ButtonSqrd/Render/VertexArray.h"

#include<glad/glad.h>

namespace BtnSqd {
	class OpenGLVertexArray: public VertexArray {
	public:
		OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuffer) override;
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuffer) override;

		const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() override;
		const std::shared_ptr<IndexBuffer>& getIndexBuffer() override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> vertexBufferList;
		std::shared_ptr<IndexBuffer> indexBuffer;
		uint32_t rendererId;
	};
}