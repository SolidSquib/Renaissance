#include "RenaissancePCH.h"

#include "Renaissance/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Renaissance::Graphics
{
	RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OpenGL;

	UniquePtr<RendererAPI> RendererAPI::Create()
	{
		switch (sAPI)
		{
			case RendererAPI::API::None:    REN_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return MakeUnique<OpenGLRendererAPI>();
		}

		REN_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}