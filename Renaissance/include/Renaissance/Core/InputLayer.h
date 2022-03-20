#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/Layer.h"
#include "Renaissance/Events/KeyEvent.h"
#include "Renaissance/Events/MouseEvent.h"

#include <map>

namespace Renaissance
{
	using namespace Events;

	class InputLayer : public Layer 
	{
	public:
		InputLayer() : Layer("InputLayer") {}
		~InputLayer() = default;

		virtual void OnAttached() override {}
		virtual void OnDetached() override {}
		virtual void OnEvent(Event& e) override;

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);

		void BindKeyPressedEventCallback(uint32_t keyCode, const EventDispatcher::EventFunction<KeyPressedEvent>& callback);
		void BindKeyReleasedEventCallback(uint32_t keyCode, const EventDispatcher::EventFunction<KeyReleasedEvent>& callback);
		void BindMouseButtonPressedEventCallback(uint32_t buttonCode, const EventDispatcher::EventFunction<MouseButtonPressedEvent>& callback);
		void BindMouseButtonReleasedEventCallback(uint32_t buttonCode, const EventDispatcher::EventFunction<MouseButtonReleasedEvent>& callback);
		void BindMouseMovedEventCallback(const EventDispatcher::EventFunction<MouseMovedEvent>& callback);
		void BindMouseScrolledEventCallback(const EventDispatcher::EventFunction<MouseScrolledEvent>& callback);

	private:
		std::multimap<uint32_t, EventDispatcher::EventFunction<KeyPressedEvent>> mKeyPressedSubscribers;
		std::multimap<uint32_t, EventDispatcher::EventFunction<KeyReleasedEvent>> mKeyReleasedSubscribers;
		std::multimap<uint32_t, EventDispatcher::EventFunction<MouseButtonPressedEvent>> mMouseButtonPressedSubscribers;
		std::multimap<uint32_t, EventDispatcher::EventFunction<MouseButtonReleasedEvent>> mMouseButtonReleasedSubscribers;
		std::vector<EventDispatcher::EventFunction<MouseMovedEvent>> mMouseMovedSubscribers;
		std::vector<EventDispatcher::EventFunction<MouseScrolledEvent>> mMouseScrolledSubscribers;
	};
}