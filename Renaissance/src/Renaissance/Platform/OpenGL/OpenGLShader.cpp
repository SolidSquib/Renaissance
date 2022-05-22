#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

#include <string.h>

namespace Renaissance::Graphics
{
	static GLenum GetShaderTypeFromString(const std::string& string)
	{
		if (string == "vertex")
			return GL_VERTEX_SHADER;
		else if (string == "fragment" || string == "pixel")
			return GL_FRAGMENT_SHADER;
		else if (string == "geometry")
			return GL_GEOMETRY_SHADER;
		else if (string == "compute")
			return GL_COMPUTE_SHADER;

		REN_CORE_ASSERT(false, "Invalid shader type specified");
		return 0;
	}

	static std::string GetStringFromShaderType(const GLenum& shaderType)
	{
		if (shaderType == GL_VERTEX_SHADER)
			return "vertex";
		else if (shaderType == GL_FRAGMENT_SHADER)
			return "fragment";
		else if (shaderType == GL_GEOMETRY_SHADER)
			return "geometry";
		else if (shaderType == GL_COMPUTE_SHADER)
			return "compute";

		REN_CORE_ASSERT(false, "Invalid shader type specified");
		return 0;
	}

	OpenGLShader::OpenGLShader(const char* filePath)
	{
		std::string sourceCode = ReadFromFile(filePath);
		Compile(PreProcess(sourceCode));
	}

	OpenGLShader::OpenGLShader(const char* vertexSource, const char* fragmentSource)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(mRendererId);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mRendererId);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(mRendererId, name.c_str()), (int)value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(mRendererId, name.c_str()), value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(mRendererId, name.c_str()), value);
	}

	void OpenGLShader::SetVector2(const std::string& name, const Vector2& vector) const
	{
		glUniform2f(glGetUniformLocation(mRendererId, name.c_str()), vector.x, vector.y);
	}

	void OpenGLShader::SetVector3(const std::string& name, const Vector3& vector) const
	{
		glUniform3f(glGetUniformLocation(mRendererId, name.c_str()), vector.x, vector.y, vector.z);
	}

	void OpenGLShader::SetVector4(const std::string& name, const Vector4& vector) const
	{
		glUniform4f(glGetUniformLocation(mRendererId, name.c_str()), vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::SetColor(const std::string& name, const Vector4& vector) const
	{
		SetVector4(name, vector);
	}

	void OpenGLShader::SetMatrix4(const std::string& name, const Matrix4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(mRendererId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	bool OpenGLShader::GetBool(const std::string& name) const
	{
		int outValue;
		glGetUniformiv(mRendererId, glGetUniformLocation(mRendererId, name.c_str()), &outValue);
		return (bool)outValue;
	}

	int OpenGLShader::GetInt(const std::string& name) const
	{
		int outValue;
		glGetUniformiv(mRendererId, glGetUniformLocation(mRendererId, name.c_str()), &outValue);
		return outValue;
	}

	float OpenGLShader::GetFloat(const std::string& name) const
	{
		float outValue;
		glGetUniformfv(mRendererId, glGetUniformLocation(mRendererId, name.c_str()), &outValue);
		return outValue;
	}

	Vector2 OpenGLShader::GetVector2(const std::string& name) const
	{
		float value[2];
		glGetnUniformfv(mRendererId, glGetUniformLocation(mRendererId, name.c_str()), 2, value);
		return Vector2(value[0], value[1]);
	}

	Vector3 OpenGLShader::GetVector3(const std::string& name) const
	{
		float value[3];
		glGetnUniformfv(mRendererId, glGetUniformLocation(mRendererId, name.c_str()), 3, value);
		return Vector3(value[0], value[1], value[2]);
	}

	Vector4 OpenGLShader::GetVector4(const std::string& name) const
	{
		float value[4];
		glGetnUniformfv(mRendererId, glGetUniformLocation(mRendererId, name.c_str()), 4, value);
		return Vector4(value[0], value[1], value[2], value[3]);
	}

	Vector4 OpenGLShader::GetColor(const std::string& name) const
	{
		return GetVector4(name);
	}

	bool OpenGLShader::CheckShaderCompilation(uint32_t OpenGLShader)
	{
		int success;
		glGetShaderiv(OpenGLShader, GL_COMPILE_STATUS, &success);
		return success;
	}

	bool OpenGLShader::CheckProgramLinkage(uint32_t program)
	{
		int success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		return success;
	}

	std::string OpenGLShader::ReadFromFile(const char* filePath)
	{
		std::ifstream in;
		std::stringstream stream;

		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// Read the contents of the shader files and save them to strings
		try
		{
			in.open(filePath);
			stream << in.rdbuf();
			in.close();
		}
		catch (std::ifstream::failure error)
		{
			REN_CORE_ERROR("Shader file read failed: {0}", error.what());
		}

		return stream.str();
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& fileSource)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = fileSource.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = fileSource.find_first_of("\r\n", pos);
			REN_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t sectionBegin = pos + typeTokenLength + 1; // Assumes only a single char (space/tab) between #type and the type name.
			std::string type = fileSource.substr(sectionBegin, eol - sectionBegin);

			size_t nextLinePos = fileSource.find_first_not_of("\r\n", eol);
			pos = fileSource.find(typeToken, nextLinePos);

			shaderSources[GetShaderTypeFromString(type)] = fileSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? fileSource.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& sources)
	{
		std::vector<unsigned int> shaders(sources.size());
		int maxLogLength;

		unsigned int program = glCreateProgram();

		for (const auto& source : sources)
		{
			const char* shaderSource = source.second.c_str();

			unsigned int shader;
			shader = glCreateShader(source.first);
			glShaderSource(shader, 1, &shaderSource, NULL);
			glCompileShader(shader);

			if (!CheckShaderCompilation(shader))
			{
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLogLength);
				std::vector<char> infoLog(maxLogLength);
				glGetShaderInfoLog(shader, maxLogLength, &maxLogLength, infoLog.data());

				REN_CORE_ERROR("Shader ({0}) compilation failed:");
				REN_CORE_ERROR("  {0}", infoLog.data());
				REN_CORE_ERROR("  Source ({0}): \n {1}", GetStringFromShaderType(source.first), shaderSource);
				
				glDeleteShader(shader);
				continue; // early-out if one component of this shader program fails to compile.
			}

			glAttachShader(program, shader);
			shaders.push_back(shader);
		}

		glLinkProgram(program);
		if (!CheckProgramLinkage(program))
		{
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLogLength);
			std::vector<char> infoLog(maxLogLength);
			glGetProgramInfoLog(program, maxLogLength, &maxLogLength, infoLog.data());
			REN_CORE_ERROR("Shader linkage failed: {0}", infoLog.data());

			glDeleteProgram(program);
			for (auto& id : shaders)
			{
				glDeleteShader(id);
			}
		}
		else // Success!
		{
			for (auto& id : shaders)
			{
				glDetachShader(program, id);
				glDeleteShader(id);
			}

			mRendererId = program;
		}
	}

	uint32_t OpenGLShader::GetOpenGLDataTypeFromShaderDataType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Int:	return GL_INT;
		case ShaderDataType::Int2:	return GL_INT;
		case ShaderDataType::Int3:	return GL_INT;
		case ShaderDataType::Int4:	return GL_INT;
		case ShaderDataType::Bool:	return GL_BOOL;
		case ShaderDataType::Mat3:	return GL_FLOAT;
		case ShaderDataType::Mat4:	return GL_FLOAT;
		}

		REN_CORE_ASSERT(false, "Unknown data type!");
		return 0;
	}
}