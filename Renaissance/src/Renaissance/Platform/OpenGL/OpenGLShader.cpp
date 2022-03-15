#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Renaissance::Graphics
{
	OpenGLShader::OpenGLShader(const char* vertexSource, const char* fragmentSource)
	{
		// Create and compile shaders, link to shader program
		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);

		int maxLogLength;
		const bool vertexShaderCompiled = CheckShaderCompilation(vertex);
		const bool fragmentShaderCompiled = CheckShaderCompilation(fragment);

		if (!vertexShaderCompiled)
		{
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLogLength);
			std::vector<char> infoLog(maxLogLength);
			glGetShaderInfoLog(vertex, maxLogLength, &maxLogLength, &infoLog[0]);

			glGetShaderiv(vertex, GL_SHADER_SOURCE_LENGTH, &maxLogLength);
			std::vector<char> source(maxLogLength);
			glGetShaderSource(vertex, maxLogLength, &maxLogLength, &source[0]);

			REN_CORE_ERROR("Vertex shader compilation failed:");
			REN_CORE_ERROR("  {0}", infoLog.data());
			REN_CORE_ERROR("  Vertex source: \n {0}", source.data());
		}

		if (!fragmentShaderCompiled)
		{
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &maxLogLength);
			std::vector<char> infoLog(maxLogLength);
			glGetShaderInfoLog(vertex, maxLogLength, &maxLogLength, &infoLog[0]);

			glGetShaderiv(vertex, GL_SHADER_SOURCE_LENGTH, &maxLogLength);
			std::vector<char> source(maxLogLength);
			glGetShaderSource(vertex, maxLogLength, &maxLogLength, &source[0]);

			REN_CORE_ERROR("Fragment shader compilation failed:");
			REN_CORE_ERROR("  {0}", infoLog.data());
			REN_CORE_ERROR("  Fragment source: \n {0}", source.data());
		}

		if (vertexShaderCompiled && fragmentShaderCompiled)
		{
			mRendererId = glCreateProgram();
			glAttachShader(mRendererId, vertex);
			glAttachShader(mRendererId, fragment);
			glLinkProgram(mRendererId);

			glDeleteShader(vertex);
			glDeleteShader(fragment);

			if (!CheckProgramLinkage(mRendererId))
			{
				glGetProgramiv(mRendererId, GL_INFO_LOG_LENGTH, &maxLogLength);
				std::vector<char> infoLog(maxLogLength);
				glGetProgramInfoLog(mRendererId, maxLogLength, &maxLogLength, &infoLog[0]);
				REN_CORE_ERROR("Shader linkage failed: {0}", infoLog.data());
			}
		}
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