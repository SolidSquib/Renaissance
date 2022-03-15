#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/VertexArray.h"

namespace Renaissance::Graphics
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual std::vector<SharedPtr<VertexBuffer>> GetVertexBuffers() const final { return mVertexBuffers; }
		virtual SharedPtr<IndexBuffer> GetIndexBuffer() const final { return mIndexBuffer; }

		virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) final;
		virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) final;

	private:
		uint32_t mRendererId = 0;
		std::vector<SharedPtr<VertexBuffer>> mVertexBuffers;
		SharedPtr<IndexBuffer> mIndexBuffer;
	};
}