#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Math/Math.h"

#include <glm/glm.hpp>
#include <string.h>

namespace Renaissance::Graphics
{
	using namespace Math;

	enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Bool, Mat3, Mat4 };

	static inline uint32_t GetDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return sizeof(float);			// 4
		case ShaderDataType::Float2: return sizeof(float) * 2;		// 8
		case ShaderDataType::Float3: return sizeof(float) * 3;		// 12
		case ShaderDataType::Float4: return sizeof(float) * 4;		// 16
		case ShaderDataType::Int:	return sizeof(int);				// 4
		case ShaderDataType::Int2:	return sizeof(int) * 2;			// 8
		case ShaderDataType::Int3:	return sizeof(int) * 3;			// 12
		case ShaderDataType::Int4:	return sizeof(int) * 4;			// 16
		case ShaderDataType::Bool:	return sizeof(bool);			// 1
		case ShaderDataType::Mat3:	return sizeof(float) * 3 * 3;	// 36
		case ShaderDataType::Mat4:	return sizeof(float) * 4 * 4;	// 64
		}

		REN_CORE_ASSERT(false, "Unknown data type!");
		return 0;
	}

	static inline uint32_t GetComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Int:	return 1;
		case ShaderDataType::Int2:	return 2;
		case ShaderDataType::Int3:	return 3;
		case ShaderDataType::Int4:	return 4;
		case ShaderDataType::Bool:	return 1;
		case ShaderDataType::Mat3:	return 3 * 3;
		case ShaderDataType::Mat4:	return 4 * 4;
		}

		REN_CORE_ASSERT(false, "Unknown data type!");
		return 0;
	}

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

		static SharedPtr<Shader> CreateFromFile(const char* vertexPath, const char* fragmentPath);
		static SharedPtr<Shader> CreateFromSource(const char* vertexSource, const char* fragmentSource);
	};
}