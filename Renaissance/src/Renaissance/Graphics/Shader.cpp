#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Renderer.h"
#include "Renaissance/Platform/OpenGL/OpenGLShader.h"

namespace Renaissance::Graphics
{
	SharedPtr<Shader> Shader::CreateFromFile(const char* sourcePath)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLShader>(sourcePath);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	SharedPtr<Shader> Shader::CreateFromSource(const char* vertexSource, const char* fragmentSource)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLShader>(vertexSource, fragmentSource);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}
}