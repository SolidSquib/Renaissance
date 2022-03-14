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

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t mRendererId;
	};

	//////////////////////////////////////////////////////////////////////////
	// Index Buffer //////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual uint32_t GetCount() const override { return mCount; }

	private:
		uint32_t mRendererId;
		uint32_t mCount;
	};
}
