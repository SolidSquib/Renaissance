#include "RenaissancePCH.h"
#include "Renaissance/Graphics/SubTexture.h"

#include "Renaissance/Graphics/Renderer.h"
#include "Renaissance/Platform/OpenGL/OpenGLSubTexture.h"

namespace Renaissance::Graphics
{
	SharedPtr<SubTexture2D> SubTexture2D::Create(const SharedPtr<Texture2D>& texture, const Vector2& min, const Vector2& max)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:	REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLSubTexture2D>(texture, min, max);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}
}