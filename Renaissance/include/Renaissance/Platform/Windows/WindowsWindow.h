#pragma once

#include "Renaissance/Core/Window.h"
#include "Renaissance/Platform/OpenGL/OpenGLContext.h"

struct GLFWwindow;

namespace Renaissance
{
	class WindowsWindow : public Window
	{
	public:		
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

		virtual void OnUpdate() final;

		virtual unsigned int GetWidth() const final { return mData.Properties.Width; }
		virtual unsigned int GetHeight() const final { return mData.Properties.Height; }

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFunction& callback) final { mData.EventCallback = callback; }
		virtual void SetVSync(bool enabled) final;
		virtual bool IsVSync() const final { return mData.VSyncEnabled; }

		virtual void* GetNativeWindow() const final { return (void*)mWindow; }

	private:
		virtual void Init(const WindowProperties& properties);
		virtual void Shutdown();

	private:
		GLFWwindow* mWindow = nullptr;
		Graphics::OpenGLContext* mRenderContext = nullptr;

		struct WindowData
		{
			WindowProperties Properties;
			int Xpos, Ypos;
			bool VSyncEnabled;
			EventCallbackFunction EventCallback;
		};

		WindowData mData;
	};
}



