#pragma once

#include "Event.h"
#include <sstream>

namespace Renaissance::Events
{
	// MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled

	class MouseButtonEvent : public Event
	{
	public:
		DEFINE_REN_EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(int buttonCode)
			: mButtonCode(buttonCode)
		{}

		int mButtonCode;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int buttonCode)
			: MouseButtonEvent(buttonCode)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed: (button code: " << mButtonCode << ")";
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int buttonCode)
			: MouseButtonEvent(buttonCode)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleased: (button code: " << mButtonCode << ")";
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(MouseButtonReleased)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: mMouseX(x), mMouseY(y)
		{}

		inline float GetMouseX() const { return mMouseX; }
		inline float GetMouseY() const { return mMouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(MouseMoved)
		DEFINE_REN_EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		float mMouseX, mMouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y)
			: mOffsetX(x), mOffsetY(y)
		{}

		inline float GetMouseX() const { return mOffsetX; }
		inline float GetMouseY() const { return mOffsetY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << mOffsetX << ", " << mOffsetY;
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(MouseScrolled)
		DEFINE_REN_EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse);

	private:
		float mOffsetX, mOffsetY;
	};
}

