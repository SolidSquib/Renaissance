#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Buffer.h"
#include "Renaissance/Graphics/Renderer.h"

#include "Renaissance/Platform/OpenGL/OpenGLBuffer.h"

namespace Renaissance::Graphics
{
	//////////////////////////////////////////////////////////////////////////
	// Vertex Buffer /////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	SharedPtr<VertexBuffer> VertexBuffer::Create(size_t size)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLVertexBuffer>(size);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	SharedPtr<VertexBuffer> VertexBuffer::Create(void* vertices, size_t size)
	{
		switch (Renderer::Get().GetAPI())
		{
			case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeShared<OpenGLVertexBuffer>(vertices, size);
			default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Index Buffer //////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	SharedPtr<IndexBuffer> IndexBuffer::Create(uint32_t count)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLIndexBuffer>(count);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	SharedPtr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::Get().GetAPI())
		{
			case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeShared<OpenGLIndexBuffer>(indices, count);
			default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}
}
