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

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t mRendererId = 0;
	};
}