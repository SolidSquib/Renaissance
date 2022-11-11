#pragma once

#include "Renaissance/Graphics/FrameBuffer.h"
#include "Renaissance/Math/Math.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	namespace Utils
	{
		static GLenum GLTextureTarget(bool multisample)
		{
			return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static GLenum GLAttachmentInternalFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
			case FrameBufferTextureFormat::RGBA16F: return GL_RGBA16F;
			case FrameBufferTextureFormat::RGBA32F: return GL_RGBA32F;
			case FrameBufferTextureFormat::RG32F: return GL_RG32F;
			case FrameBufferTextureFormat::RED_INTEGER: return GL_R32I;
			case FrameBufferTextureFormat::RED_INTEGER_UNSIGNED: return GL_R32UI;
			case FrameBufferTextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
			case FrameBufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
			default:
				REN_CORE_ASSERT(false);
				return GL_RGBA8;
			}
		}

		static GLenum GLAttachmentFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::RGBA8: return GL_RGBA;
			case FrameBufferTextureFormat::RGBA16F: return GL_RGBA;
			case FrameBufferTextureFormat::RGBA32F: return GL_RGBA;
			case FrameBufferTextureFormat::RG32F: return GL_RG;
			case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			case FrameBufferTextureFormat::RED_INTEGER_UNSIGNED: return GL_RED_INTEGER;
			default:
				REN_CORE_ASSERT(false);
				return GL_RGBA;
			}
		}

		static GLenum GLAttachmentType(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::DEPTH32F: return GL_DEPTH_ATTACHMENT;
			case FrameBufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
			default:
				return GL_COLOR_ATTACHMENT0;
			}
		}

		static void GLCreateTextures(bool multisample, uint32_t* outId, uint32_t count)
		{
			glCreateTextures(GLTextureTarget(multisample), count, outId);
		}

		static void GLCreateRenderBuffers(bool multisample, uint32_t* outId, uint32_t count)
		{
			glCreateRenderbuffers(count, outId);
		}

		static void GLBindTexture(bool multisample, uint32_t id)
		{
			glBindTexture(GLTextureTarget(multisample), id);
		}

		static void GLAttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				//glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
			}

			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GLTextureTarget(multisampled), id, 0);
		}

		static void GLAttachDepthTexture(uint32_t id, int samples, GLenum internalFormat, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
			}

			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GLTextureTarget(multisampled), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GLTextureTarget(multisampled), id, 0);
		}
	}

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public: 
		OpenGLFrameBuffer(const Specification& specification);
		virtual ~OpenGLFrameBuffer();

		virtual void Resize(uint32_t width, uint32_t height) final;
		virtual void Invalidate() final;
		void Destroy();

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual const Specification& GetSpecification() const final { return mSpecification; }
		virtual uint32_t GetNumColorAttachments() const final { return (uint32_t)mColorAttachmentSpecs.size(); }
		virtual bool HasDepthAttachment() const final { return mDepthAttachmentSpec.TextureFormat != FrameBufferTextureFormat::None; }
		virtual uint32_t GetColorAttachmentRendererId(uint32_t index) const final;
		virtual uint32_t GetDepthAttachmentRendererId() const final;

		virtual int ReadPixel(uint32_t index, uint32_t x, uint32_t y) const final;
		virtual void ClearAttachment(uint32_t, const Math::Vector4& clearColor) final;

	private:
		uint32_t mRendererId = 0;
		Specification mSpecification;
		std::vector<SharedPtr<Texture2D>> mTextures;

		std::vector<FrameBufferTextureSpecification> mColorAttachmentSpecs;
		FrameBufferTextureSpecification mDepthAttachmentSpec;

		std::vector<uint32_t> mColorAttachments;
		uint32_t mDepthAttachment;
	};
}