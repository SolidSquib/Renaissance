#pragma once

#include <string>
#include "Renaissance/Events/Event.h"

using namespace Renaissance::Events;

namespace Renaissance
{
	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(const std::string& title = "Renaissance Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	// Interface to represent a desktop window
	class REN_API Window
	{
	public: 
		using EventCallbackFunction = std::is_function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void IsVSync() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}


