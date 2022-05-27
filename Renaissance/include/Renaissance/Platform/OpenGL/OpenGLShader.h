#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/Shader.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();	

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual void SetBool(const std::string& name, bool value) const final;
		virtual void SetInt(const std::string& name, int value) const final;
		virtual void SetFloat(const std::string& name, float value) const final;
		virtual void SetVector2(const std::string& name, const Vector2& vector) const final;
		virtual void SetVector3(const std::string& name, const Vector3& vector) const final;
		virtual void SetVector4(const std::string& name, const Vector4& vector) const final;
		virtual void SetColor(const std::string& name, const Vector4& color) const final;
		virtual void SetMatrix4(const std::string& name, const Matrix4& matrix) const final;

		virtual bool GetBool(const std::string& name) const final;
		virtual int GetInt(const std::string& name) const final;
		virtual float GetFloat(const std::string& name) const final;
		virtual Vector2 GetVector2(const std::string& name) const final;
		virtual Vector3 GetVector3(const std::string& name) const final;
		virtual Vector4 GetVector4(const std::string& name) const final;
		virtual Vector4 GetColor(const std::string& name) const final;

		virtual std::string GetName() const final { return mName; }

		static bool SendDataAsInteger(ShaderDataType type);
		static uint32_t GetOpenGLDataTypeFromShaderDataType(ShaderDataType type);

	private:
		uint32_t mRendererId = 0;
		std::string mName;

		static bool CheckShaderCompilation(uint32_t shader);
		static bool CheckProgramLinkage(uint32_t program);

		std::string ReadFromFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& fileSource);
		void Compile(const std::unordered_map<GLenum, std::string>& sources);
	};
}