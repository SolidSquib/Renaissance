#include "RenaissancePCH.h"
#include "Renaissance/Graphics/FrameBuffer.h"

#include "Renaissance/Graphics/Renderer.h"
#include "Renaissance/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Renaissance::Graphics
{	
	SharedPtr<FrameBuffer> FrameBuffer::Create(const Specification& specification, const FrameBufferLayout& layout)
	{
		switch (Renderer::Get().GetAPI())
		{
			case RendererAPI::API::None:	REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	return MakeShared<OpenGLFrameBuffer>(specification, layout);
			default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}
}