#pragma once

#include "Event.h"

#include <sstream>

namespace Renaissance::Events
{
	class KeyEvent : public Event
	{
	public:
		inline uint32_t GetKeyCode() const { return mKeyCode; }

		DEFINE_REN_EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		KeyEvent(uint32_t keyCode)
			: mKeyCode(keyCode)
		{}

		uint32_t mKeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(uint32_t keyCode, unsigned int repeatCount)
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
		KeyReleasedEvent(uint32_t keyCode)
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