#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLVertexArray.h"
#include "Renaissance/Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &mRendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &mRendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(mRendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer)
	{
		REN_CORE_ASSERT(vertexBuffer.get(), "Vertex buffer is invalid!");

		glBindVertexArray(mRendererId);
		vertexBuffer->Bind();

		for (uint32_t i = 0; i < vertexBuffer->GetLayout().GetCount(); ++i)
		{
			BufferElement element = vertexBuffer->GetLayout()[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i,
				GetComponentCount(element.Type),
				OpenGLShader::GetOpenGLDataTypeFromShaderDataType(element.Type),
				element.Normalized,
				vertexBuffer->GetLayout().GetStride(),
				(const void*)(uint64_t)element.Offset);
		}

		mVertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer)
	{
		REN_CORE_ASSERT(indexBuffer.get(), "Index buffer is invalid!");

		glBindVertexArray(mRendererId);
		indexBuffer->Bind();

		mIndexBuffer = indexBuffer;
	}

}