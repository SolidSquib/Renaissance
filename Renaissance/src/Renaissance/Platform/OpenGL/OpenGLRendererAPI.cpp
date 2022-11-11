#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Renaissance/Graphics/VertexArray.h"

#include <glad/glad.h>

namespace Renaissance::Graphics
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         REN_CORE_FATAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       REN_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          REN_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: REN_CORE_TRACE(message); return;
		}

		REN_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
	#ifdef REN_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const Vector4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear(uint32_t flags)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const SharedPtr<VertexArray>& vertexArray)
	{
		DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
	}

	void OpenGLRendererAPI::DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}