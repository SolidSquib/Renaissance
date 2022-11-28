#pragma once

#include "Renaissance/Core/Window.h"
#include "Renaissance/Platform/OpenGL/OpenGLContext.h"

struct GLFWwindow;

namespace Renaissance
{
	class WindowsWindow : public Window
	{
	public:		
		WindowsWindow(const WindowProperties& properties, int xPos, int yPos);
		virtual ~WindowsWindow();

		virtual void OnUpdate() final;

		virtual uint32_t GetWidth() const final { return mData.Properties.Width; }
		virtual uint32_t GetHeight() const final { return mData.Properties.Height; }
		virtual uint32_t GetPositionX() const final { return mData.Xpos; }
		virtual uint32_t GetPositionY() const final { return mData.Ypos; }
		virtual uint32_t GetIndex() const final { return mData.Index; }

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFunction& callback) final { mData.EventCallback = callback; }
		virtual void SetVSync(bool enabled) final;
		virtual void SetPosition(uint32_t x, uint32_t y) final;
		virtual bool IsVSync() const final { return mData.VSyncEnabled; }

		virtual void* GetNativeWindow() const final { return (void*)mWindow; }

	protected:
		virtual void SetIndex(uint32_t index) final { mData.Index = index; }

	private:
		virtual void Init(const WindowProperties& properties, int x, int y);
		virtual void Shutdown();

	private:
		GLFWwindow* mWindow = nullptr;
		Graphics::OpenGLContext* mRenderContext = nullptr;

		struct WindowData
		{
			WindowProperties Properties;
			int Xpos = 0, Ypos = 0;
			bool VSyncEnabled = true;
			EventCallbackFunction EventCallback;
			uint32_t Index = 0;
		};

		WindowData mData;
	};
}



