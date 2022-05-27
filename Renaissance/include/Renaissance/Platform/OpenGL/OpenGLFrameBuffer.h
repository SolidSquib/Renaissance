#pragma once

#include "Renaissance/Graphics/FrameBuffer.h"

namespace Renaissance::Graphics
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public: 
		OpenGLFrameBuffer(const Specification& specification);
		virtual ~OpenGLFrameBuffer();

		virtual void Resize(uint32_t width, uint32_t height) final;
		virtual void Invalidate() final;

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual const Specification& GetSpecification() const final { return mSpecification; }
		virtual uint32_t GetColorAttachmentRendererId() const final { return mColorAttachment; }

	private:
		void Destroy();

		uint32_t mRendererId;
		uint32_t mColorAttachment, mDepthStencilAttachment;
		Specification mSpecification;
	};
}