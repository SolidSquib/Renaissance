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

		virtual void OnUpdate() override;

		virtual unsigned int GetWidth() const override { return mData.Properties.Width; }
		virtual unsigned int GetHeight() const override { return mData.Properties.Height; }

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFunction& callback) override { mData.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override { return mData.VSyncEnabled; }

		virtual void* GetNativeWindow() const override { return (void*)mWindow; }

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



