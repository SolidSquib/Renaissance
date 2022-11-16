#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Texture.h"
#include "Renaissance/Graphics/Renderer.h"

#include "Renaissance/Platform/OpenGL/OpenGLTexture.h"

namespace Renaissance::Graphics
{
	SharedPtr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLTexture2D>(width, height);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	SharedPtr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data)
	{
		switch (Renderer::Get().GetAPI())
		{
		case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return MakeShared<OpenGLTexture2D>(width, height, data);
		default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}
	}

	SharedPtr<Texture2D> Texture2D::Create(const String& path)
	{
		SharedPtr<Texture2D> newTexture;
		REN_CORE_ASSERT(!TextureLibrary::GetGlobal().Exists(path));

		switch (Renderer::Get().GetAPI())
		{
			case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	newTexture = MakeShared<OpenGLTexture2D>(path); break;
			default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}

		TextureLibrary::GetGlobal().Add(path, newTexture);
		return newTexture;
	}

	SharedPtr<Texture3D> Texture3D::Create(const String& path)
	{
		SharedPtr<Texture3D> newTexture;
		REN_CORE_ASSERT(!TextureLibrary::GetGlobal().Exists(path));

		switch (Renderer::Get().GetAPI())
		{
			case RendererAPI::API::None:		REN_CORE_ASSERT(false, "Running without a renderer is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:	newTexture = MakeShared<OpenGLTexture3D>(path); break;
			default: REN_CORE_ASSERT(false, "Unknown rendering API specified!"); return nullptr;
		}

		TextureLibrary::GetGlobal().Add(path, newTexture);
		return newTexture;
	}

	UniquePtr<TextureLibrary> TextureLibrary::sInstance = MakeUnique<TextureLibrary>();

	void TextureLibrary::Add(const std::string& name, const SharedPtr<Texture2D>& texture)
	{
		REN_CORE_ASSERT(!Exists(name));
		mTextures[name] = texture;
	}

	void TextureLibrary::Add(const std::string& name, const SharedPtr<Texture3D>& texture)
	{
		REN_CORE_ASSERT(!Exists(name));
		mTextures[name] = texture;
	}

	bool TextureLibrary::Exists(const std::string& name) const
	{
		return mTextures.find(name) != mTextures.end();
	}
}
