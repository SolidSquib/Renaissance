#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &mRendererId);
		Bind();
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
}