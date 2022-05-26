#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Renderer.h"
#include "Renaissance/Platform/OpenGL/OpenGLShader.h"

namespace Renaissance::Graphics
{
	SharedPtr<Shader> Shader::CreateFromFile(const std::string& sourcePath)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLShader>(sourcePath);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	SharedPtr<Shader> Shader::CreateFromSource(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLShader>(name, vertexSource, fragmentSource);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	UniquePtr<ShaderLibrary> ShaderLibrary::sInstance = MakeUnique<ShaderLibrary>();

	SharedPtr<Shader> ShaderLibrary::Get(const std::string& name) const
	{
		REN_CORE_ASSERT(Exists(name));
		return mShaders.at(name);
	}

	SharedPtr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		if (!Exists(name))
		{
			SharedPtr<Shader> shader = Shader::CreateFromFile(filePath);
			Add(name, shader);
			return shader;
		}

		REN_CORE_INFO("Shader with name \"{0}\" already exists", name);
		return Get(name);
	}

	SharedPtr<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		SharedPtr<Shader> shader = Shader::CreateFromFile(filePath);

		if (!Exists(shader->GetName()))
		{
			Add(shader);
			return shader;
		}

		REN_CORE_WARN("Shader with name \"{0}\" already exists. The loaded shader was not added to the library.", shader->GetName());
		return shader;
	}

	void ShaderLibrary::Add(const std::string& name, const SharedPtr<Shader>& shader)
	{
		REN_CORE_ASSERT(!Exists(name));
		mShaders[name] = shader;
	}

	void ShaderLibrary::Add(const SharedPtr<Shader>& shader)
	{
		Add(shader->GetName(), shader);
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return mShaders.find(name) != mShaders.end();
	}

}