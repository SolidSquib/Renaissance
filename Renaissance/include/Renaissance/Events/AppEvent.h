#pragma once

#include "Event.h"

namespace Renaissance::Events
{
	// AppTick, AppUpdate, AppRender

	class AppEvent : public Event
	{
	public:
		DEFINE_REN_EVENT_CATEGORY(EventCategoryApplication)

	protected:
		AppEvent() {}
	};

	class AppTickEvent : public AppEvent
	{
	public:
		AppTickEvent() : AppEvent() {}

		DEFINE_REN_EVENT_TYPE(AppTick)
	};

	class AppUpdateEvent : public AppEvent
	{
	public:
		AppUpdateEvent() : AppEvent() {}

		DEFINE_REN_EVENT_TYPE(AppUpdate)
	};

	class AppRenderEvent : public AppEvent
	{
	public:
		AppRenderEvent() : AppEvent() {}

		DEFINE_REN_EVENT_TYPE(AppUpdate)
	};
}