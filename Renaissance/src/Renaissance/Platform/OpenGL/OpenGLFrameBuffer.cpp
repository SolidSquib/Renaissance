#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const Specification& specification)
		: mSpecification(specification)
	{		
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		Destroy();
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		mSpecification.Width = width;
		mSpecification.Height = height;
		Invalidate();
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		Destroy();

		glCreateFramebuffers(1, &mRendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);

		glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
		glTextureStorage2D(mColorAttachment, 1, GL_RGB8, mSpecification.Width, mSpecification.Height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

		glCreateRenderbuffers(1, &mDepthStencilAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilAttachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mSpecification.Width, mSpecification.Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilAttachment);

		REN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Destroy()
	{
		if (mRendererId != 0)
		{
			glDeleteFramebuffers(1, &mRendererId);
		}
	}
}