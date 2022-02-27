#include "RenaissancePCH.h"
#include "Renaissance/Platform/Windows/WindowsWindow.h"
#include "GLFW/glfw3.h"

namespace Renaissance
{
	static uint8_t sGLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* desc)
	{
		REN_CORE_ERROR("GLFW error {0}: {1}", error, desc);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(mWindow);		
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		mData.VSyncEnabled = enabled;
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		mData.Properties = properties;

		REN_CORE_INFO("Creating window \"{0}\" with dimensions:  {1}, {2}", properties.Title, properties.Width, properties.Height);
		if (sGLFWWindowCount == 0)
		{
			int success = glfwInit();
			REN_CORE_ASSERT(success, "Failed to initialize GLFW.");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		mWindow = glfwCreateWindow((int)properties.Width, (int)properties.Height, properties.Title.c_str(), nullptr, nullptr);
		sGLFWWindowCount++;
	}

	void WindowsWindow::Shutdown()
	{

	}
}