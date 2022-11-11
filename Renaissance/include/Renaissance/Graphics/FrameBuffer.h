#pragma once

#include "Renaissance/Graphics/Texture.h"
#include "Renaissance/Math/Math.h"

namespace Renaissance::Graphics
{
	enum class FrameBufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RGBA16F,
		RGBA32F,
		RG32F,
		RED_INTEGER,
		RED_INTEGER_UNSIGNED,

		// Depth/Stencil
		DEPTH32F,
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	namespace Utils
	{
		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::DEPTH32F:
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				return true;
			}

			return false;
		}
	}

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format, bool isWritable = true)
			: TextureFormat(format), IsRenderBuffer(!isWritable) {}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
		Math::Vector4 ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		bool IsRenderBuffer;
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(const std::initializer_list<FrameBufferTextureSpecification>& attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	class FrameBuffer
	{
	public:
		struct Specification
		{
			uint32_t Width = 0, Height = 0;
			uint32_t Samples = 1;
			FrameBufferAttachmentSpecification Attachments;
			bool SwapChainTarget = false;
		};

	public:
		virtual ~FrameBuffer() = default;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Invalidate() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const Specification& GetSpecification() const = 0;
		virtual uint32_t GetNumColorAttachments() const = 0;
		virtual bool HasDepthAttachment() const = 0;
		virtual uint32_t GetColorAttachmentRendererId(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentRendererId() const = 0;

		virtual int ReadPixel(uint32_t index, uint32_t x, uint32_t y) const = 0;
		virtual void ClearAttachment(uint32_t index, const Math::Vector4& clearColor) = 0;

		static SharedPtr<FrameBuffer> Create(const Specification& specification);
	};
}