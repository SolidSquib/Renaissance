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

		uint32_t GetWindowIndex() const { return mIndex; }

	protected:
		WindowEvent(uint32_t index) : mIndex(index) {}

		uint32_t mIndex;
	};

	class WindowClosedEvent : public WindowEvent
	{
	public:
		WindowClosedEvent(uint32_t index) : WindowEvent(index) {}

		DEFINE_REN_EVENT_TYPE(WindowClose)
	};

	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(uint32_t index, uint32_t width, uint32_t height, bool maximized)
			: WindowEvent(index), mWidth(width), mHeight(height), mIsMaximized(maximized)
		{}

		inline uint32_t GetWidth() const { return mWidth; }
		inline uint32_t GetHeight() const { return mHeight; }
		inline bool IsMaximized() const { return mIsMaximized; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(WindowResize)

	private:
		uint32_t mWidth, mHeight;
		bool mIsMaximized;
	};

	class WindowFocusEvent : public WindowEvent
	{
	public:
		WindowFocusEvent(uint32_t index) : WindowEvent(index) {}

		DEFINE_REN_EVENT_TYPE(WindowFocus)
	};

	class WindowLostFocusEvent : public WindowEvent
	{
	public:
		WindowLostFocusEvent(uint32_t index) : WindowEvent(index) {}

		DEFINE_REN_EVENT_TYPE(WindowLostFocus)
	};

	class WindowMovedEvent: public WindowEvent
	{
	public:
		WindowMovedEvent(uint32_t index, int xpos, int ypos)
			: WindowEvent(index), mX(xpos), mY(ypos)
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