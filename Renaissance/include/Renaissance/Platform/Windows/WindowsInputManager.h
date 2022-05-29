#pragma once

#include "Renaissance/Core/InputManager.h"

namespace Renaissance
{
	class WindowsInputManager : public InputManager
	{
	protected:
		virtual bool IsKeyPressedImpl(unsigned int keyCode) const final;
		virtual bool IsMouseButtonPressedImpl(unsigned int buttonCode) const final;
		virtual std::pair<float, float> GetMousePositionImpl() const final;
		virtual void SetMousePositionImpl(float x, float y) final;
		virtual void EnableMouseCursorImpl() const final;
		virtual void DisableMouseCursorImpl() const final;
	};
}
