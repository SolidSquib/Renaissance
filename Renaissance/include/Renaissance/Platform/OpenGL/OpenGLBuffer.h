#pragma once

#include "Renaissance/Graphics/Buffer.h"

namespace Renaissance::Graphics
{
	//////////////////////////////////////////////////////////////////////////
	// Vertex Buffer /////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(size_t size);
		OpenGLVertexBuffer(void* vertices, size_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual void SetData(void* data, size_t size, size_t offset) final;

		inline virtual size_t GetSize() const final { return mSize; }
		inline virtual BufferLayout GetLayout() const final { return mLayout; }
		inline virtual void SetLayout(const BufferLayout& layout) final { mLayout = layout; }

	private:
		size_t mSize;
		uint32_t mRendererId;
		BufferLayout mLayout;
	};

	//////////////////////////////////////////////////////////////////////////
	// Index Buffer //////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count);
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual void SetData(uint32_t* data, uint32_t count, size_t offset) final;

		inline virtual uint32_t GetCount() const final { return mCount; }

	private:
		uint32_t mRendererId;
		uint32_t mCount;
	};
}
