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

	class TextureLibrary
	{
	public:
		template <typename T>
		SharedPtr<T> Get(const std::string& name) const
		{
			REN_CORE_ASSERT(Exists(name));
			SharedPtr<T> castPtr = std::static_pointer_cast<T>(mTextures.at(name));
			REN_CORE_ASSERT(castPtr);
			return castPtr;
		}

		template <typename T>
		SharedPtr<T> Load(const std::string& name, const std::filesystem::path& filePath)
		{
			if (!Exists(name))
			{
				SharedPtr<T> texture = T::Create(filePath);
				Add(name, texture);
				return texture;
			}

			REN_CORE_INFO("Texture with name \"{0}\" already exists", name);
			return Get<T>(name);
		}

		template <typename T>
		SharedPtr<T> Load(const std::filesystem::path& filePath)
		{
			SharedPtr<T> texture = T::Create(filePath);

			if (!Exists(texture->GetName()))
			{
				Add(filePath.stem(), texture);
				return texture;
			}

			REN_CORE_WARN("Texture with name \"{0}\" already exists. The loaded texture was not added to the library.", shader->GetName());
			return shader;
		}

		void Add(const std::string& name, const SharedPtr<Texture2D>& texture);
		void Add(const std::string& name, const SharedPtr<Texture3D>& texture);

		bool Exists(const std::string& name) const;

		static TextureLibrary& GetGlobal() { return *sInstance; }

	private:
		std::unordered_map<std::string, SharedPtr<Texture>> mTextures;

		static UniquePtr<TextureLibrary> sInstance;
	};
}