#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/Shader.h"

namespace Renaissance::Graphics
{
	//////////////////////////////////////////////////////////////////////////
	// BufferLayout //////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	struct BufferElement
	{
		ShaderDataType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Type(type), Name(name), Size(GetDataTypeSize(type)), Offset(0), Normalized(normalized)
		{}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& initializer)
			: mElements(initializer)
		{
			Init();
		}

		inline void Init()
		{
			mStride = 0;
			for (uint32_t i = 0; i < mElements.size(); ++i)
			{
				mElements[i].Offset = mStride;
				mStride += mElements[i].Size;
			}
		}

		inline std::vector<BufferElement> GetElements() const { return mElements; }
		inline uint32_t GetStride() const { return mStride; }
		inline uint32_t GetCount() const { return (uint32_t)mElements.size(); }

		inline void SetElements(const std::vector<BufferElement> elements) { mElements = elements; Init(); }

		inline std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return mElements.end(); }
		inline std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

		inline const BufferElement& operator[](uint32_t index) const { return mElements[index]; }

	private:
		std::vector<BufferElement> mElements;
		uint32_t mStride;
	};

	//////////////////////////////////////////////////////////////////////////
	// Vertex Buffer /////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual BufferLayout GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static SharedPtr<VertexBuffer> Create(float* vertices, size_t size);
	};

	//////////////////////////////////////////////////////////////////////////
	// Index Buffer //////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static SharedPtr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}