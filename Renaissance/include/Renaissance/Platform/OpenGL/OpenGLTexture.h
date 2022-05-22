#pragma once

#include "Renaissance/Graphics/Texture.h"

namespace Renaissance::Graphics
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const String& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		uint32_t mWidth, mHeight;
		uint32_t mRendererId;
		String mPath;
	};

	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D(const String& path);
		virtual ~OpenGLTexture3D();

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		uint32_t mWidth, mHeight;
		uint32_t mRendererId;
		String mPath;
	};
}