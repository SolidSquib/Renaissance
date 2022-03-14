#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/Shader.h"

namespace Renaissance::Graphics
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const char* vertexSource, const char* fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetBool(const std::string& name, bool value) const override;
		virtual void SetInt(const std::string& name, int value) const override;
		virtual void SetFloat(const std::string& name, float value) const override;
		virtual void SetVector2(const std::string& name, const Vector2& vector) const override;
		virtual void SetVector3(const std::string& name, const Vector3& vector) const override;
		virtual void SetVector4(const std::string& name, const Vector4& vector) const override;
		virtual void SetColor(const std::string& name, const Vector4& color) const override;
		virtual void SetMatrix4(const std::string& name, const Matrix4& matrix) const override;

		virtual bool GetBool(const std::string& name) const override;
		virtual int GetInt(const std::string& name) const override;
		virtual float GetFloat(const std::string& name) const override;
		virtual Vector2 GetVector2(const std::string& name) const override;
		virtual Vector3 GetVector3(const std::string& name) const override;
		virtual Vector4 GetVector4(const std::string& name) const override;
		virtual Vector4 GetColor(const std::string& name) const override;

	private:
		uint32_t mRendererId = 0;

		static bool CheckShaderCompilation(uint32_t shader);
		static bool CheckProgramLinkage(uint32_t program);
	};
}