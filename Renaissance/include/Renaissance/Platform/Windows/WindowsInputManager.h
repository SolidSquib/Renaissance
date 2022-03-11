#pragma once

#include "Renaissance/Core/InputManager.h"

namespace Renaissance
{
	class WindowsInputManager : public InputManager
	{
	protected:
		virtual bool IsKeyPressedImpl(unsigned int keyCode) const override;
		virtual bool IsMouseButtonPressedImpl(unsigned int buttonCode) const override;
		virtual std::pair<float, float> GetMousePositionImpl() const override;
	};
}
