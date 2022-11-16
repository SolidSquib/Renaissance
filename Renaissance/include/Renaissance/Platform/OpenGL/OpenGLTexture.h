#pragma once

#include "Renaissance/Graphics/Texture.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const String& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(uint32_t width, uint32_t height, void* data);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const final { return mWidth; }
		virtual uint32_t GetHeight() const final { return mHeight; }
		virtual uint32_t GetRendererId() const final { return mRendererId; }
		virtual std::filesystem::path GetPath() const final { return mPath; };

		virtual void SetData(void* data, uint32_t size) final;

		virtual void Bind(uint32_t slot = 0) const final;

		virtual bool operator==(const Texture& other) const final
		{
			return mRendererId == ((OpenGLTexture2D&)other).mRendererId;
		}

	private:
		uint32_t mWidth = 0, mHeight = 0;
		uint32_t mRendererId = 0;
		String mPath = "";
		GLenum mInternalFormat = GL_RGB8;
		GLenum mDataFormat = GL_RGB;
	};

	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D(const String& path);
		virtual ~OpenGLTexture3D();

		virtual uint32_t GetWidth() const final { return mWidth; }
		virtual uint32_t GetHeight() const final { return mHeight; }
		virtual uint32_t GetRendererId() const final { return mRendererId; }
		virtual std::filesystem::path GetPath() const final { return mPath; };

		virtual void SetData(void* data, uint32_t size) final;

		virtual void Bind(uint32_t slot = 0) const final;

		virtual bool operator==(const Texture& other) const final
		{
			return mRendererId == ((OpenGLTexture3D&)other).mRendererId;
		}

	private:
		uint32_t mWidth = 0, mHeight = 0;
		uint32_t mRendererId = 0;
		String mPath = "";
	};
}