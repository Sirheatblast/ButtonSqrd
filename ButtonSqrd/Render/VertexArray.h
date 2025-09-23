#pragma once
#include"Buffers.h"

#include<memory>

namespace BtnSqd {
	class VertexArray {
	public:
		virtual ~VertexArray() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() = 0;
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() = 0;

		static VertexArray* Create();
	};
}