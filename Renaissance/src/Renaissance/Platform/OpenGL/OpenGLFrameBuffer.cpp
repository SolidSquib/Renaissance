#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Renaissance::Graphics
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const Specification& specification)
		: mSpecification(specification)
	{
		for (auto& attachment : specification.Attachments.Attachments)
		{
			if (Utils::IsDepthFormat(attachment.TextureFormat))
			{
				REN_CORE_ASSERT(mDepthAttachmentSpec.TextureFormat == FrameBufferTextureFormat::None, "A framebuffer may only have one depth attachment.");
				mDepthAttachmentSpec = attachment;
			}
			else
			{
				mColorAttachmentSpecs.emplace_back(attachment);
			}
		}

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

	void OpenGLFrameBuffer::Destroy()
	{
		glDeleteFramebuffers(1, &mRendererId);		
		glDeleteTextures((GLsizei)mColorAttachments.size(), mColorAttachments.data());		
		glDeleteTextures(1, &mDepthAttachment);

		mColorAttachments.clear();
		mRendererId = 0;
		mDepthAttachment = 0;
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		REN_PROFILE_FUNCTION()

		Destroy();

		glCreateFramebuffers(1, &mRendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);

		bool multisample = mSpecification.Samples > 1;

		if (mColorAttachmentSpecs.size())
		{
			mColorAttachments.resize(mColorAttachmentSpecs.size());
			Utils::GLCreateTextures(multisample, mColorAttachments.data(), (uint32_t)mColorAttachments.size());

			for (uint32_t i = 0; i < mColorAttachments.size(); ++i)
			{
				Utils::GLBindTexture(multisample, mColorAttachments[i]);
				GLenum internalFormat = Utils::GLAttachmentInternalFormat(mColorAttachmentSpecs[i].TextureFormat);
				GLenum readFormat = Utils::GLAttachmentFormat(mColorAttachmentSpecs[i].TextureFormat);
				Utils::GLAttachColorTexture(mColorAttachments[i], mSpecification.Samples, internalFormat, readFormat, mSpecification.Width, mSpecification.Height, i);
			}
		}

		if (mDepthAttachmentSpec.TextureFormat != FrameBufferTextureFormat::None)
		{
			Utils::GLCreateTextures(multisample, &mDepthAttachment, 1);
			Utils::GLBindTexture(multisample, mDepthAttachment);
			GLenum internalFormat = Utils::GLAttachmentInternalFormat(mDepthAttachmentSpec.TextureFormat);
			GLenum attachmentType = Utils::GLAttachmentType(mDepthAttachmentSpec.TextureFormat);
			Utils::GLAttachDepthTexture(mDepthAttachment, mSpecification.Samples, internalFormat, attachmentType, mSpecification.Width, mSpecification.Height);
		}

		if (mColorAttachments.size() > 1)
		{
			REN_CORE_ASSERT(mColorAttachments.size() <= 4, "Renaissance currently only supports a maximum of 4 color attachments per framebuffer");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)mColorAttachments.size(), buffers);
		}
		else if (mColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		REN_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		REN_PROFILE_FUNCTION()

		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);
		glViewport(0, 0, mSpecification.Width, mSpecification.Height);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		REN_PROFILE_FUNCTION()

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererId(uint32_t index) const
	{
		if (index < mColorAttachments.size())
		{
			return mColorAttachments[index];
		}

		REN_CORE_WARN("Unable to find color attachment at index {0}", index);
		return 0;
	}

	uint32_t OpenGLFrameBuffer::GetDepthAttachmentRendererId() const
	{
		return mDepthAttachment;
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t index, uint32_t x, uint32_t y) const
	{
		REN_CORE_ASSERT(index < mColorAttachments.size());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);

		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t index, const Math::Vector4& clearColor)
	{
		REN_CORE_ASSERT(index < mColorAttachments.size());

		switch (mColorAttachmentSpecs[index].TextureFormat)
		{
		case FrameBufferTextureFormat::RGBA8: 
		case FrameBufferTextureFormat::RGBA16F: 
		case FrameBufferTextureFormat::RGBA32F:
		case FrameBufferTextureFormat::RG32F:
		{
			float color[] = {clearColor.r, clearColor.g, clearColor.b, clearColor.a};
			glClearBufferfv(GL_COLOR, index, color);
		}
		break;
		case FrameBufferTextureFormat::RED_INTEGER: 
		{
			int color[] = { (int)clearColor.r, (int)clearColor.g, (int)clearColor.b, (int)clearColor.a };
			glClearBufferiv(GL_COLOR, index, color);
		}
		break;
		case FrameBufferTextureFormat::RED_INTEGER_UNSIGNED: 
		{
			uint32_t color[] = { (uint32_t)clearColor.r, (uint32_t)clearColor.g, (uint32_t)clearColor.b, (uint32_t)clearColor.a };
			glClearBufferuiv(GL_COLOR, index, color);
		}
		break;
		default:
			REN_CORE_ASSERT(false);
		}
	}
}
