#pragma once

#include <string>
#include "Renaissance/Events/Event.h"

using namespace Renaissance::Events;

namespace Renaissance
{
	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool Maximized;

		WindowProperties(const std::string& title = "Renaissance Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			bool maximized = true)
			: Title(title), Width(width), Height(height), Maximized(maximized)
		{}
	};

	// Interface to represent a desktop window
	class Window
	{
	public: 
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetPositionX() const = 0;
		virtual uint32_t GetPositionY() const = 0;
		virtual uint32_t GetIndex() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetPosition(uint32_t x, uint32_t y) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static UniquePtr<Window> Create(const WindowProperties& properties = WindowProperties(), int xPos = 0, int yPos = 0);

	protected:
		virtual void SetIndex(uint32_t index) = 0;

		friend class Application;
	};
}


