#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/InputCodes.h"
#include "Renaissance/Core/InputLayer.h"
#include "Renaissance/Events/KeyEvent.h"

#include <map>

namespace Renaissance
{
	using namespace Events; 
	class InputManager
	{
	public:
		enum class InputEventType { Pressed, Released };

	public:
		virtual void Init();
		virtual void Shutdown() {}

		static bool IsKeyPressed(uint32_t keyCode) { return sInstance->IsKeyPressedImpl(keyCode); }
		static bool IsMouseButtonPressed(uint32_t buttonCode) { return sInstance->IsMouseButtonPressedImpl(buttonCode); }
		static std::pair<float, float> GetMousePosition() { return sInstance->GetMousePositionImpl(); }
		static float GetMouseX() { return sInstance->GetMouseXImpl(); }
		static float GetMouseY() { return sInstance->GetMouseYImpl(); }

		// currently unsafe since there's no way to unbind an event when a subscriber's lifetime comes to an end.
		static void BindKeyPressedEventCallback(uint32_t keyCode, const EventDispatcher::EventFunction<KeyPressedEvent>& callback) { sInstance->mInputLayer.lock()->BindKeyPressedEventCallback(keyCode, callback); }
		static void BindKeyReleasedEventCallback(uint32_t keyCode, const EventDispatcher::EventFunction<KeyReleasedEvent>& callback) { sInstance->mInputLayer.lock()->BindKeyReleasedEventCallback(keyCode, callback); }
		static void BindMouseButtonPressedEventCallback(uint32_t buttonCode, const EventDispatcher::EventFunction<MouseButtonPressedEvent>& callback) { sInstance->mInputLayer.lock()->BindMouseButtonPressedEventCallback(buttonCode, callback); }
		static void BindMouseButtonReleasedEventCallback(uint32_t buttonCode, const EventDispatcher::EventFunction<MouseButtonReleasedEvent>& callback) { sInstance->mInputLayer.lock()->BindMouseButtonReleasedEventCallback(buttonCode, callback); }
		static void BindMouseMovedEventCallback(const EventDispatcher::EventFunction<MouseMovedEvent>& callback) { sInstance->mInputLayer.lock()->BindMouseMovedEventCallback(callback); }
		static void BindMouseScrolledEventCallback(const EventDispatcher::EventFunction<MouseScrolledEvent>& callback) { sInstance->mInputLayer.lock()->BindMouseScrolledEventCallback(callback); }

		static inline InputManager& Get() { return *sInstance; }

	protected:
		virtual bool IsKeyPressedImpl(uint32_t keyCode) const = 0;
		virtual bool IsMouseButtonPressedImpl(uint32_t buttonCode) const = 0;
		virtual std::pair<float, float> GetMousePositionImpl() const = 0;
		virtual float GetMouseXImpl() const;
		virtual float GetMouseYImpl() const;

	private:
		static InputManager* sInstance;

		WeakPtr<InputLayer> mInputLayer;
	};
}