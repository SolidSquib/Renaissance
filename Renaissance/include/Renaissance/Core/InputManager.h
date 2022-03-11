#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/InputCodes.h"

namespace Renaissance
{
	class InputManager
	{
	public:
		static bool IsKeyPressed(unsigned int keyCode) { return sInstance->IsKeyPressedImpl(keyCode); }
		static bool IsMouseButtonPressed(unsigned int buttonCode) { return sInstance->IsMouseButtonPressedImpl(buttonCode); }
		static std::pair<float, float> GetMousePosition() { return sInstance->GetMousePositionImpl(); }
		static float GetMouseX() { return sInstance->GetMouseXImpl(); }
		static float GetMouseY() { return sInstance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(unsigned int keyCode) const = 0;
		virtual bool IsMouseButtonPressedImpl(unsigned int buttonCode) const = 0;
		virtual std::pair<float, float> GetMousePositionImpl() const = 0;
		virtual float GetMouseXImpl() const;
		virtual float GetMouseYImpl() const;

	private:
		static InputManager* sInstance;
	};
}