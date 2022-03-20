#include "Sandbox/ExampleLayer.h"
#include "Renaissance/Core/InputManager.h"

namespace Sandbox
{
	void ExampleLayer::OnAttached()
	{
		Renaissance::InputManager::BindKeyPressedEventCallback(Renaissance::KeyCode::Tab, REN_BIND_EVENT(ExampleLayer::OnTabKeyPressed));
	}

	void ExampleLayer::OnDetached()
	{
		
	}

	bool ExampleLayer::OnTabKeyPressed(Renaissance::Events::KeyPressedEvent& e)
	{
		REN_TRACE("Tab key pressed!");
		return true;
	}
}