#pragma once

#include "Renaissance/Core/Core.h"

#include <functional>

namespace Renaissance::Events
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		EventCategoryNone = 0,
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
		EventCategoryWindow = BIT(5),
		EventCategoryApplication = BIT(6)
	};

	#define DEFINE_REN_EVENT_TYPE(Type) static EventType GetStaticType() { return EventType::##Type; } \
		virtual EventType GetEventType() const override { return GetStaticType(); } \
		virtual const char* GetName() const override { return #Type; }

	#define DEFINE_REN_EVENT_CATEGORY(Categories) virtual unsigned int GetCategoryFlags() const override { return Categories; }

	class Event
	{
	public:
		bool mHandled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual unsigned int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool HasCategory(unsigned int cat) const
		{
			return GetCategoryFlags() & cat;
		}

		inline bool IsHandled() const { return mHandled; }
	};

	class EventDispatcher
	{
		/* Currently implements blocking events, but should refactor this to use a buffer at
		* some point so we can throw events in a more controlled/ordered way */
	public:
		template<typename T>
		using EventFunction = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: mEvent(event)
		{}

		template <typename T>
		bool Dispatch(EventFunction<T> function)
		{
			if (mEvent.GetEventType() == T::GetStaticType())
			{
				mEvent.mHandled |= function(static_cast<T&>(mEvent));
				return true;
			}
			return false;
		}

	private:
		Event& mEvent;
	};

	//inline std::ostream& operator<<(std::ostream& os, const Event& e)
	//{
	//	return os << e.ToString();
	//}
}
