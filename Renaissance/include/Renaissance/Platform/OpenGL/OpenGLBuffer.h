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
		OpenGLVertexBuffer(float* vertices, size_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual BufferLayout GetLayout() const final { return mLayout; }
		virtual void SetLayout(const BufferLayout& layout) final { mLayout = layout; }

	private:
		uint32_t mRendererId;
		BufferLayout mLayout;
	};

	//////////////////////////////////////////////////////////////////////////
	// Index Buffer //////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const final;
		virtual void Unbind() const final;

		inline virtual uint32_t GetCount() const final { return mCount; }

	private:
		uint32_t mRendererId;
		uint32_t mCount;
	};
}
