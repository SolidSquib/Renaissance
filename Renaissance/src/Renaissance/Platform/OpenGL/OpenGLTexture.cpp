#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{

	OpenGLTexture2D::OpenGLTexture2D(const String& path)
		: mPath(path)
	{
		int width, height, channels;
		
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		REN_CORE_ASSERT(data, "Failed to load image from file.");

		mWidth = width;
		mHeight = height;

		uint32_t dataFormat = 0, internalFormat;

		switch (channels)
		{
		case 1: dataFormat = GL_R; internalFormat = GL_R8; break;
		case 2: dataFormat = GL_RG; internalFormat = GL_RG8; break;
		case 3: dataFormat = GL_RGB; internalFormat = GL_RGB8; break;
		case 4: dataFormat = GL_RGBA; internalFormat = GL_RGBA8; break;
		default: REN_CORE_ASSERT(!"Unsupported number of texture channels in texture loaded from file: {0}", path.c_str());
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererId);
		glTextureStorage2D(mRendererId, 1, internalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(mRendererId, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &mRendererId);
	}

	void OpenGLTexture2D::Bind(uint32_t slot /*= 0*/) const
	{
		glBindTexture(GL_TEXTURE0 + slot, mRendererId);
	}

	OpenGLTexture3D::OpenGLTexture3D(const String& path)
		: mPath(path)
	{

	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		glDeleteTextures(1, &mRendererId);
	}

	void OpenGLTexture3D::Bind(uint32_t slot /*= 0*/) const
	{
		glBindTexture(GL_TEXTURE0 + slot, mRendererId);
	}

}