#pragma once

#include "Renaissance/Graphics/FrameBuffer.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public: 
		OpenGLFrameBuffer(const Specification& specification, const FrameBufferLayout& layout);
		virtual ~OpenGLFrameBuffer();

		virtual void Resize(uint32_t width, uint32_t height) final;
		virtual void Invalidate() final;

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual const Specification& GetSpecification() const final { return mSpecification; }
		virtual uint32_t GetNumAttachmentsOfType(FrameBufferAttachmentType type) const final { return mComponentCount.find(type)->second; }
		virtual uint32_t GetAttachmentRendererId(FrameBufferAttachmentType type, uint32_t index) const final;

	private:
		bool ValidateAttachmentOfType(FrameBufferAttachmentType type);
		GLenum GetGLAttachmentFormatEnum(FrameBufferAttachmentType type);
		GLenum GetGLAttachmentTypeEnum(FrameBufferAttachmentType type);

		uint32_t mRendererId;
		Specification mSpecification;
		FrameBufferLayout mLayout;
		std::unordered_map<FrameBufferAttachmentType, uint32_t> mComponentCount;
		std::vector<SharedPtr<Texture2D>> mTextures;
	};
}