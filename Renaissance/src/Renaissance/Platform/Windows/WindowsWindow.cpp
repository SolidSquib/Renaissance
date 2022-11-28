#include "RenaissancePCH.h"
#include "Renaissance/Platform/Windows/WindowsWindow.h"

// Events
#include "Renaissance/Events/WindowEvent.h"
#include "Renaissance/Events/KeyEvent.h"
#include "Renaissance/Events/MouseEvent.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Renaissance
{
	static uint8_t sGLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* desc)
	{
		REN_CORE_ERROR("GLFW error {0}: {1}", error, desc);
		REN_CORE_ASSERT(false);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties, int xPos, int yPos)
	{
		Init(properties, xPos, yPos);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		mRenderContext->SwapBuffers();
		glfwPollEvents();
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

	void WindowsWindow::SetPosition(uint32_t x, uint32_t y)
	{
		HWND windowHandle = glfwGetWin32Window(mWindow);
	
		if (IsZoomed(windowHandle))
		{
			glfwRestoreWindow(mWindow);
		}

		int left, top, right, bottom;
		glfwGetWindowFrameSize(mWindow, &left, &top, &right, &bottom);
		glfwSetWindowPos(mWindow, x, y + top);
	}

	void WindowsWindow::Init(const WindowProperties& properties, int x, int y)
	{
		mData.Properties = properties;
		mData.Index = sGLFWWindowCount;

		REN_CORE_INFO("Creating window \"{0}\" with dimensions:  {1}, {2}", mData.Properties.Title, mData.Properties.Width, mData.Properties.Height);
		if (sGLFWWindowCount == 0)
		{
			int success = glfwInit();
			REN_CORE_ASSERT(success, "Failed to initialize GLFW.");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_MAXIMIZED, mData.Properties.Maximized ? GLFW_TRUE : GLFW_FALSE);
		mWindow = glfwCreateWindow((int)mData.Properties.Width, (int)mData.Properties.Height, mData.Properties.Title.c_str(), nullptr, nullptr);
		
		if (!mData.Properties.Maximized)
		{
			SetPosition(x, y);
		}

		sGLFWWindowCount += 1;

		mRenderContext = new Graphics::OpenGLContext(mWindow);
		mRenderContext->Init();

		SetVSync(true);

		glfwSetWindowUserPointer(mWindow, &mData);
		glfwGetWindowPos(mWindow, &mData.Xpos, &mData.Ypos);

		// GLFW event callbacks:
		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.EventCallback(Events::WindowClosedEvent(data.Index));
		});

		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Properties.Width = width;
			data.Properties.Height = height;
			HWND nativeWindow = glfwGetWin32Window(window);
			data.Properties.Maximized = IsZoomed(nativeWindow);

			data.EventCallback(Events::WindowResizeEvent(data.Index, static_cast<uint32_t>(width), static_cast<uint32_t>(height), data.Properties.Maximized));
		});

		glfwSetWindowFocusCallback(mWindow, [](GLFWwindow* window, int focused) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (focused == GLFW_TRUE)
			{
				data.EventCallback(Events::WindowFocusEvent(data.Index));
			}
			else
			{
				data.EventCallback(Events::WindowLostFocusEvent(data.Index));
			}			
		});

		glfwSetJoystickCallback([](int joystickId, int event) {

		});

		glfwSetWindowPosCallback(mWindow, [](GLFWwindow* window, int xpos, int ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			int left, top, right, bottom;
			glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);

			data.Xpos = xpos;
			data.Ypos = ypos - top;
			data.EventCallback(Events::WindowMovedEvent(data.Index, xpos, ypos));
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
		delete mRenderContext;
		mRenderContext = nullptr;

		glfwDestroyWindow(mWindow);
		mWindow = nullptr;

		sGLFWWindowCount -= 1;
	}
}