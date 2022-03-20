#include "RenaissancePCH.h"
#include "Renaissance/Core/InputManager.h"
#include "Renaissance/Core/Application.h"

namespace Renaissance
{

	void InputManager::Init()
	{
		mInputLayer = Application::Get().CreateNewLayer<InputLayer>();
	}

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
