#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Renderer.h"
#include "Renaissance/Platform/OpenGL/OpenGLShader.h"

namespace Renaissance::Graphics
{
	SharedPtr<Shader> Shader::CreateFromFile(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode, fragmentCode;
		std::ifstream vertexFile, fragmentFile;
		std::stringstream vertexStream, fragmentStream;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// Read the contents of the shader files and save them to strings
		try
		{
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);			

			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			return CreateFromSource(vertexStream.str().c_str(), fragmentStream.str().c_str());
		}
		catch (std::ifstream::failure error)
		{
			REN_CORE_ERROR("Shader file read failed: {0}", error.what());
			return nullptr;
		}
	}

	SharedPtr<Shader> Shader::CreateFromSource(const char* vertexSource, const char* fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:	return MakeShared<OpenGLShader>(vertexSource, fragmentSource);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}
}