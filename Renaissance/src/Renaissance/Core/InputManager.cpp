#include "RenaissancePCH.h"
#include "InputManager.h"

namespace Renaissance
{
	float InputManager::GetMouseXImpl() const
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float InputManager::GetMouseYImpl() const
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}
