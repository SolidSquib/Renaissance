#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLTexture.h"

#include "stb_image.h"

namespace Renaissance::Graphics
{
	uint32_t GetDataFormatSize(GLenum format)
	{
		switch (format)
		{
		case GL_R: return 1;
		case GL_RG: return 2;
		case GL_RGB: return 3;
		case GL_RGBA: return 4;
		default: REN_CORE_ASSERT(!"Unsupported data format"); return 0;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: mWidth(width), mHeight(height), mInternalFormat(GL_RGBA8), mDataFormat(GL_RGBA)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererId);
		glTextureStorage2D(mRendererId, 1, mInternalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void* data)
		: OpenGLTexture2D(width, height)
	{
		SetData(data, mWidth * mHeight * GetDataFormatSize(mDataFormat));
	}

	OpenGLTexture2D::OpenGLTexture2D(const String& path)
		: mPath(path)
	{
		int width, height, channels;
		
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		REN_CORE_ASSERT(data, "Failed to load image from file.");

		mWidth = width;
		mHeight = height;

		switch (channels)
		{
		case 1: mDataFormat = GL_R; mInternalFormat = GL_R8; break;
		case 2: mDataFormat = GL_RG; mInternalFormat = GL_RG8; break;
		case 3: mDataFormat = GL_RGB; mInternalFormat = GL_RGB8; break;
		case 4: mDataFormat = GL_RGBA; mInternalFormat = GL_RGBA8; break;
		default: REN_CORE_ASSERT(!"Unsupported number of texture channels in texture loaded from file: {0}", path.c_str());
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererId);
		glTextureStorage2D(mRendererId, 1, mInternalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(mRendererId, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &mRendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		REN_CORE_ASSERT(size == mWidth * mHeight * GetDataFormatSize(mDataFormat), "SetData only supports setting full texture in OpenGL mode.");
		glTextureSubImage2D(mRendererId, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot /*= 0*/) const
	{
		glBindTextureUnit(slot, mRendererId);
	}

	OpenGLTexture3D::OpenGLTexture3D(const String& path)
		: mPath(path)
	{
		REN_CORE_ASSERT(false, "Not yet implemented");
	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		glDeleteTextures(1, &mRendererId);
	}

	void OpenGLTexture3D::SetData(void* data, uint32_t size)
	{

	}

	void OpenGLTexture3D::Bind(uint32_t slot /*= 0*/) const
	{
		glBindTextureUnit(slot, mRendererId);
	}

}