#pragma once

#include "Event.h"

#include <sstream>

namespace Renaissance::Events
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return mKeyCode; }

		DEFINE_REN_EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		KeyEvent(int keyCode)
			: mKeyCode(keyCode)
		{}

		int mKeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, unsigned int repeatCount)
			: KeyEvent(keyCode), mRepeatCount(repeatCount)
		{}

		inline unsigned int GetRepeatCount() const { return mRepeatCount; }
		inline bool IsRepeatPress() const { return mRepeatCount > 0; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << "repeats)";
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(KeyPressed)

	protected:
		unsigned int mRepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode)
		{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << mKeyCode;
			return ss.str();
		}

		DEFINE_REN_EVENT_TYPE(KeyReleased)
	};
}