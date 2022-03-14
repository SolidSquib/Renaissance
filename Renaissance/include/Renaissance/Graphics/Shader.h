#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Math/Math.h"

#include <glm/glm.hpp>
#include <string.h>

namespace Renaissance::Graphics
{
	using namespace Math;

	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBool(const std::string& name, bool value) const = 0;
		virtual void SetInt(const std::string& name, int value) const = 0;
		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetVector2(const std::string& name, const Vector2& vector) const = 0;
		virtual void SetVector3(const std::string& name, const Vector3& vector) const = 0;
		virtual void SetVector4(const std::string& name, const Vector4& vector) const = 0;
		virtual void SetColor(const std::string& name, const Vector4& color) const = 0;
		virtual void SetMatrix4(const std::string& name, const Matrix4& matrix) const = 0;

		virtual bool GetBool(const std::string& name) const = 0;
		virtual int GetInt(const std::string& name) const = 0;
		virtual float GetFloat(const std::string& name) const = 0;
		virtual Vector2 GetVector2(const std::string& name) const = 0;
		virtual Vector3 GetVector3(const std::string& name) const = 0;
		virtual Vector4 GetVector4(const std::string& name) const = 0;
		virtual Vector4 GetColor(const std::string& name) const = 0;

		static Shader* CreateFromFile(const char* vertexPath, const char* fragmentPath);
		static Shader* CreateFromSource(const char* vertexSource, const char* fragmentSource);
	};
}