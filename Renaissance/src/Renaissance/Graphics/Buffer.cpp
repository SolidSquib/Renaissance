#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Buffer.h"
#include "Renaissance/Graphics/Renderer.h"

#include "Renaissance/Platform/OpenGL/OpenGLBuffer.h"

namespace Renaissance::Graphics
{
	VertexBuffer* VertexBuffer::Create(float* vertices, size_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
			default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLIndexBuffer(indices, count);
			default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}
}