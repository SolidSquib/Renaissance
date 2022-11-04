#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Renaissance::Graphics
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const Specification& specification, const FrameBufferLayout& layout)
		: mSpecification(specification), mLayout(layout)
	{
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &mRendererId);

		for (auto iter = mLayout.begin(); iter != mLayout.end(); ++iter)
		{
			if (iter->RendererId)
			{
				if (iter->Writeable)
				{
					glDeleteTextures(1, &iter->RendererId);
				}
				else
				{
					glDeleteRenderbuffers(1, &iter->RendererId);
				}
			}
		}
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		mSpecification.Width = width;
		mSpecification.Height = height;
		Invalidate();
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		REN_PROFILE_FUNCTION()
					
		if (mRendererId != 0)
		{
			glDeleteFramebuffers(1, &mRendererId);
			mRendererId = 0;
		}

		for (auto iter = mLayout.begin(); iter != mLayout.end(); ++iter)
		{
			if (iter->RendererId)
			{
				if (iter->Writeable)
				{
					glDeleteTextures(1, &iter->RendererId);
				}
				else
				{
					glDeleteRenderbuffers(1, &iter->RendererId);
				}
			}
		}

		mComponentCount[FrameBufferAttachmentType::Color] = 0;
		mComponentCount[FrameBufferAttachmentType::Depth] = 0;
		mComponentCount[FrameBufferAttachmentType::Stencil] = 0;
		mComponentCount[FrameBufferAttachmentType::DepthStencil] = 0;

		glCreateFramebuffers(1, &mRendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);

		uint32_t colorCount = 0, depthCount = 0, stencilCount = 0, depthStencilCount= 0;

		for (auto iter = mLayout.begin(); iter != mLayout.end(); ++iter)
		{
			if (!ValidateAttachmentOfType(iter->Type))
			{
				REN_CORE_WARN("Unable to create framebuffer attachment of type {0}, limit reached.", GetFrameBufferAttachmentTypeName(iter->Type));
				continue;
			}

			if (iter->Writeable)
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &iter->RendererId);
				glTextureStorage2D(iter->RendererId, 1, GetGLAttachmentFormatEnum(iter->Type), mSpecification.Width, mSpecification.Height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GetGLAttachmentTypeEnum(iter->Type), GL_TEXTURE_2D, iter->RendererId, 0);
			}
			else
			{
				glCreateRenderbuffers(1, &iter->RendererId);
				glBindRenderbuffer(GL_RENDERBUFFER, iter->RendererId);
				glRenderbufferStorage(GL_RENDERBUFFER, GetGLAttachmentFormatEnum(iter->Type), mSpecification.Width, mSpecification.Height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GetGLAttachmentTypeEnum(iter->Type), GL_RENDERBUFFER, iter->RendererId);
			}

			mComponentCount.find(iter->Type)->second++;
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
	
	uint32_t OpenGLFrameBuffer::GetAttachmentRendererId(FrameBufferAttachmentType type, uint32_t index) const
	{
		uint32_t foundIndex = 0;
		for (auto iter = mLayout.begin(); iter != mLayout.end(); ++iter)
		{
			if (iter->Type == type && foundIndex == index)
			{
				return iter->RendererId;
			}
			foundIndex++;
		}

		REN_CORE_WARN("Unable to find FrameBufferAttachment of type {0}, RendererId will be invalid!", GetFrameBufferAttachmentTypeName(type));
		return 0;
	}

	bool OpenGLFrameBuffer::ValidateAttachmentOfType(FrameBufferAttachmentType type)
	{
		bool returnValue = true;
		uint32_t currentCount = mComponentCount.find(type)->second;

		switch (type)
		{
		case FrameBufferAttachmentType::Color: 
			returnValue = currentCount < 32; // get the actual max from the GPU
			break;

		case FrameBufferAttachmentType::Depth: 
		case FrameBufferAttachmentType::Stencil: 
			returnValue = currentCount < 1 
				&& mComponentCount.find(FrameBufferAttachmentType::DepthStencil)->second < 1;
			break;

		case FrameBufferAttachmentType::DepthStencil: 
			returnValue = currentCount < 1
				&& mComponentCount.find(FrameBufferAttachmentType::Depth)->second < 1
				&& mComponentCount.find(FrameBufferAttachmentType::Stencil)->second < 1;
			break;

		default: REN_CORE_ASSERT(false, "Unknown attachment type!"); break;
		}

		return returnValue;
	}

	GLenum OpenGLFrameBuffer::GetGLAttachmentFormatEnum(FrameBufferAttachmentType type)
	{
		switch (type)
		{
		case FrameBufferAttachmentType::Color: return GL_RGB8;
		case FrameBufferAttachmentType::Depth: return GL_DEPTH_COMPONENT24;
		case FrameBufferAttachmentType::Stencil: return GL_STENCIL_INDEX8;
		case FrameBufferAttachmentType::DepthStencil: return GL_DEPTH24_STENCIL8;
		default: REN_CORE_ASSERT(false, "Unknown attachment type!"); return 0;
		}
	}

	GLenum OpenGLFrameBuffer::GetGLAttachmentTypeEnum(FrameBufferAttachmentType type)
	{
		switch (type)
		{
		case FrameBufferAttachmentType::Color: return GL_COLOR_ATTACHMENT0 + mComponentCount.find(type)->second;
		case FrameBufferAttachmentType::Depth: return GL_DEPTH_ATTACHMENT;
		case FrameBufferAttachmentType::Stencil: return GL_STENCIL_ATTACHMENT;
		case FrameBufferAttachmentType::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
		default: REN_CORE_ASSERT(false, "Unknown attachment type!"); return 0;
		}
	}
}
