#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Math/Math.h"

#include <glm/glm.hpp>

namespace Renaissance::Graphics
{
	using namespace Math;

	enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Uint, Uint2, Uint3, Uint4, Bool, Mat3, Mat4 };

	static inline uint32_t GetDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	return sizeof(float);			// 4
		case ShaderDataType::Float2: return sizeof(float) * 2;		// 8
		case ShaderDataType::Float3: return sizeof(float) * 3;		// 12
		case ShaderDataType::Float4: return sizeof(float) * 4;		// 16
		case ShaderDataType::Int:	return sizeof(int32_t);			// 4
		case ShaderDataType::Int2:	return sizeof(int32_t) * 2;		// 8
		case ShaderDataType::Int3:	return sizeof(int32_t) * 3;		// 12
		case ShaderDataType::Int4:	return sizeof(int32_t) * 4;		// 16
		case ShaderDataType::Uint:	return sizeof(uint32_t);		// 4
		case ShaderDataType::Uint2:	return sizeof(uint32_t) * 2;	// 8
		case ShaderDataType::Uint3:	return sizeof(uint32_t) * 3;	// 12
		case ShaderDataType::Uint4:	return sizeof(uint32_t) * 4;	// 16
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
		case ShaderDataType::Uint:	return 1;
		case ShaderDataType::Uint2:	return 2;
		case ShaderDataType::Uint3:	return 3;
		case ShaderDataType::Uint4:	return 4;
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
		virtual ~Shader() = default;

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

		virtual std::string GetName() const = 0;

		static SharedPtr<Shader> CreateFromFile(const std::string& sourcePath);
		static SharedPtr<Shader> CreateFromSource(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};

	class ShaderLibrary
	{
	public:
		SharedPtr<Shader> Get(const std::string& name) const;

		SharedPtr<Shader> Load(const std::string& name, const std::string& filePath);
		SharedPtr<Shader> Load(const std::string& filePath);
		void Add(const std::string& name, const SharedPtr<Shader>& shader);
		void Add(const SharedPtr<Shader>& shader);

		bool Exists(const std::string& name) const;

		static ShaderLibrary& GetGlobal() { return *sInstance; }

	private:
		std::unordered_map<std::string, SharedPtr<Shader>> mShaders;

		static UniquePtr<ShaderLibrary> sInstance;
	};
}