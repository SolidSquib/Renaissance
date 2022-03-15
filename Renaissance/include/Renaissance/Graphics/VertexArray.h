#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/Buffer.h"

namespace Renaissance::Graphics
{
	class VertexArray
	{
	public:
		~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual std::vector<SharedPtr<VertexBuffer>> GetVertexBuffers() const = 0;
		virtual SharedPtr<IndexBuffer> GetIndexBuffer() const = 0;

		virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) = 0;

		static SharedPtr<VertexArray> Create();
	};
}