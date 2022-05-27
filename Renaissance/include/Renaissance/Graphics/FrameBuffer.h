#pragma once

namespace Renaissance::Graphics
{
	class FrameBuffer
	{
	public:
		struct Specification
		{
			uint32_t Width, Height;
			uint32_t Samples = 1;
			bool SwapChainTarget = false;
		};

	public:
		~FrameBuffer() = default;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Invalidate() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const Specification& GetSpecification() const = 0;
		virtual uint32_t GetColorAttachmentRendererId() const = 0;

		static SharedPtr<FrameBuffer> Create(const Specification& specification);
	};
}