#pragma once

#include "Renaissance/Core/Window.h"

struct GLFWwindow;

namespace Renaissance
{
	class MacWindow : public Window
	{
	public:		
		MacWindow(const WindowProperties& properties);
		virtual ~MacWindow();

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
		GLFWwindow* mWindow;

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



