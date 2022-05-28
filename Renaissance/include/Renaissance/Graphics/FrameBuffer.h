#pragma once

#include "Renaissance/Graphics/Texture.h"

namespace Renaissance::Graphics
{
	enum class FrameBufferAttachmentType { Color, Depth, Stencil, DepthStencil };

	static inline const char* GetFrameBufferAttachmentTypeName(FrameBufferAttachmentType type)
	{
		switch (type)
		{
		case FrameBufferAttachmentType::Color: return "Color";
		case FrameBufferAttachmentType::Depth: return "Depth";
		case FrameBufferAttachmentType::Stencil: return "Stencil";
		case FrameBufferAttachmentType::DepthStencil: return "Depth/Stencil";
		default: REN_CORE_ASSERT(false, "Unknown attachment type!"); return "Attachment unknown";
		}
	}

	struct FrameBufferAttachment
	{
		uint32_t RendererId;
		FrameBufferAttachmentType Type;
		bool Readable;
	
		FrameBufferAttachment() = default;
		FrameBufferAttachment(FrameBufferAttachmentType type, bool readable)
			: Type(type), Readable(readable)
		{ }
	};

	class FrameBufferLayout
	{
	public:
		FrameBufferLayout() = default;
		FrameBufferLayout(const std::initializer_list<FrameBufferAttachment>& initializer)
			: mAttachments(initializer)
		{
		}

		inline const std::vector<FrameBufferAttachment>& GetAttachments() const { return mAttachments; }
		inline uint32_t GetCount() const { return (uint32_t)mAttachments.size(); }

		inline void SetAttachments(const std::vector<FrameBufferAttachment>& attachments) { mAttachments = attachments; }

		inline std::vector<FrameBufferAttachment>::iterator begin() { return mAttachments.begin(); }
		inline std::vector<FrameBufferAttachment>::const_iterator begin() const { return mAttachments.begin(); }
		inline std::vector<FrameBufferAttachment>::iterator end() { return mAttachments.end(); }
		inline std::vector<FrameBufferAttachment>::const_iterator end() const { return mAttachments.end(); }

	private:
		std::vector<FrameBufferAttachment> mAttachments;
	};

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
		virtual uint32_t GetNumAttachmentsOfType(FrameBufferAttachmentType type) const = 0;
		virtual uint32_t GetAttachmentRendererId(FrameBufferAttachmentType type, uint32_t index = 0) const = 0;

		static SharedPtr<FrameBuffer> Create(const Specification& specification, const FrameBufferLayout& layout);
	};
}