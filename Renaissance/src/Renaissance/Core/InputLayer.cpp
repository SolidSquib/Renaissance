#include "RenaissancePCH.h"
#include "Renaissance/Core/InputLayer.h"
#include "Renaissance/Events/Event.h"
#include "Renaissance/Core/InputManager.h"

namespace Renaissance
{
	void InputLayer::OnEvent(Events::Event& e)
	{
		Events::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(REN_BIND_EVENT(InputLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(REN_BIND_EVENT(InputLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(REN_BIND_EVENT(InputLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(REN_BIND_EVENT(InputLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(REN_BIND_EVENT(InputLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(REN_BIND_EVENT(InputLayer::OnMouseScrolledEvent));		
	}

	bool InputLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		auto& range = mKeyPressedSubscribers.equal_range(e.GetKeyCode());
		for (auto& iter = range.first; iter != range.second; ++iter)
		{
			iter->second(e);
		}

		return true;
	}

	bool InputLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		auto& range = mKeyReleasedSubscribers.equal_range(e.GetKeyCode());
		for (auto& iter = range.first; iter != range.second; ++iter)
		{
			iter->second(e);
		}

		return true;
	}

	bool InputLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		auto& range = mMouseButtonPressedSubscribers.equal_range(e.GetButtonCode());
		for (auto& iter = range.first; iter != range.second; ++iter)
		{
			iter->second(e);
		}

		return true;
	}

	bool InputLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		auto& range = mMouseButtonReleasedSubscribers.equal_range(e.GetButtonCode());
		for (auto& iter = range.first; iter != range.second; ++iter)
		{
			iter->second(e);
		}

		return true;
	}

	bool InputLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		for (auto& iter : mMouseMovedSubscribers)
		{
			iter(e);
		}

		return true;
	}

	bool InputLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		for (auto& iter : mMouseScrolledSubscribers)
		{
			iter(e);
		}

		return true;
	}

	void InputLayer::BindKeyPressedEventCallback(uint32_t keyCode, const EventDispatcher::EventFunction<KeyPressedEvent>& callback)
	{
		mKeyPressedSubscribers.emplace(keyCode, callback);
	}

	void InputLayer::BindKeyReleasedEventCallback(uint32_t keyCode, const EventDispatcher::EventFunction<KeyReleasedEvent>& callback)
	{
		mKeyReleasedSubscribers.emplace(keyCode, callback);
	}

	void InputLayer::BindMouseButtonPressedEventCallback(uint32_t buttonCode, const EventDispatcher::EventFunction<MouseButtonPressedEvent>& callback)
	{
		mMouseButtonPressedSubscribers.emplace(buttonCode, callback);
	}

	void InputLayer::BindMouseButtonReleasedEventCallback(uint32_t buttonCode, const EventDispatcher::EventFunction<MouseButtonReleasedEvent>& callback)
	{
		mMouseButtonReleasedSubscribers.emplace(buttonCode, callback);
	}

	void InputLayer::BindMouseMovedEventCallback(const EventDispatcher::EventFunction<MouseMovedEvent>& callback)
	{
		mMouseMovedSubscribers.push_back(callback);
	}

	void InputLayer::BindMouseScrolledEventCallback(const EventDispatcher::EventFunction <MouseScrolledEvent> & callback)
	{
		mMouseScrolledSubscribers.push_back(callback);
	}
}