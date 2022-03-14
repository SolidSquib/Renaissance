#include "RenaissancePCH.h"
#include "Renaissance/Platform/Windows/WindowsWindow.h"

// Events
#include "Renaissance/Events/WindowEvent.h"
#include "Renaissance/Events/KeyEvent.h"
#include "Renaissance/Events/MouseEvent.h"

#include <GLFW/glfw3.h>

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
		mRenderContext->SwapBuffers();
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

		mRenderContext = new Graphics::OpenGLContext(mWindow);
		mRenderContext->Init();

		SetVSync(true);

		glfwSetWindowUserPointer(mWindow, &mData);
		glfwGetWindowPos(mWindow, &mData.Xpos, &mData.Ypos);

		// GLFW event callbacks:
		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.EventCallback(Events::WindowClosedEvent());
		});

		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Properties.Width = width;
			data.Properties.Height = height;
			data.EventCallback(Events::WindowResizeEvent(width, height));			
		});

		glfwSetWindowFocusCallback(mWindow, [](GLFWwindow* window, int focused) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (focused == GLFW_TRUE)
			{
				data.EventCallback(Events::WindowFocusEvent());
			}
			else
			{
				data.EventCallback(Events::WindowLostFocusEvent());
			}			
		});

		glfwSetWindowPosCallback(mWindow, [](GLFWwindow* window, int xpos, int ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Xpos = xpos;
			data.Ypos = ypos;
			data.EventCallback(Events::WindowMovedEvent(xpos, ypos));
		});

		glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					data.EventCallback(Events::KeyPressedEvent(key, 0));
					break;
				}
				case GLFW_RELEASE:
				{
					data.EventCallback(Events::KeyReleasedEvent(key));
					break;
				}
				case GLFW_REPEAT:
				{
					data.EventCallback(Events::KeyPressedEvent(key, 1));
					break;
				}
			}
		});

		glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.EventCallback(Events::MouseScrolledEvent((float)xoffset, (float)yoffset));
		});

		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.EventCallback(Events::MouseMovedEvent((float)xpos, (float)ypos));
		});

		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					data.EventCallback(Events::MouseButtonPressedEvent(button));
					break;
				}
				case GLFW_RELEASE:
				{
					data.EventCallback(Events::MouseButtonReleasedEvent(button));
					break;
				}
			}
		});
	}

	void WindowsWindow::Shutdown()
	{
		
	}
}