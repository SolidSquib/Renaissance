#pragma once

#include "Event.h"
#include <sstream>

namespace Renaissance::Events
{
	// WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved

	class WindowEvent : public Event
	{
	public:		
		DEFINE_REN_EVENT_CATEGORY(EventCategoryWindow)

	protected:
		WindowEvent() {}
	};

	class WindowClosedEvent : public WindowEvent
	{
	public:
		WindowClosedEvent() : WindowEvent() {}

		DEFINE_REN_EVENT_TYPE(WindowClose)
	};

	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: WindowEvent(), mWidth(width), mHeight(height)
		{}

		inline uint32_t GetWidth() const { return mWidth; }
		inline uint32_t GetHeight() const { return mHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(WindowResize)

	private:
		uint32_t mWidth, mHeight;
	};

	class WindowFocusEvent : public WindowEvent
	{
	public:
		WindowFocusEvent() : WindowEvent() {}

		DEFINE_REN_EVENT_TYPE(WindowFocus)
	};

	class WindowLostFocusEvent : public WindowEvent
	{
	public:
		WindowLostFocusEvent() : WindowEvent() {}

		DEFINE_REN_EVENT_TYPE(WindowLostFocus)
	};

	class WindowMovedEvent: public WindowEvent
	{
	public:
		WindowMovedEvent(int xpos, int ypos)
			: WindowEvent(), mX(xpos), mY(ypos)
		{}

		inline int GetXPos() const { return mX; }
		inline int GetYPos() const { return mY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << mX << ", " << mY;
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(WindowMoved)

	private:
		int mX, mY;
	};
}