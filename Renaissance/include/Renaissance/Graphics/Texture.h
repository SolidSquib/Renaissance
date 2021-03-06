#pragma once

#include "Renaissance/Core/Core.h"

namespace Renaissance::Graphics
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererId() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static SharedPtr<Texture2D> Create(uint32_t width, uint32_t height);
		static SharedPtr<Texture2D> Create(uint32_t width, uint32_t height, void* data);
		static SharedPtr<Texture2D> Create(const String& path);
	};

	class Texture3D : public Texture
	{
	public:
		static SharedPtr<Texture3D> Create(const String& path);
	};
}