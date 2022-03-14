#include "RenaissancePCH.h"
#include "Renaissance/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Renaissance::Graphics
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: mWindowHandle(windowHandle)
	{
		REN_CORE_ASSERT(mWindowHandle, "mWindowHandle is null.");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(mWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		REN_CORE_ASSERT(status, "Glad loader failed to initialize.");

		REN_CORE_INFO("OpenGL driver info:");
		REN_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		REN_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		REN_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::Resize(float width, float height)
	{
		glViewport(0, 0, (int)width, (int)height);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindowHandle);
	}
}