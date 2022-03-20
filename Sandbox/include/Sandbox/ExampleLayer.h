#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/Layer.h"
#include "Renaissance/Events/KeyEvent.h"

namespace Sandbox
{
	class ExampleLayer : public Renaissance::Layer
	{
	public:
		virtual void OnAttached() override;
		virtual void OnDetached() override;
		virtual void OnEvent(Renaissance::Events::Event& e) override { }

		bool OnTabKeyPressed(Renaissance::Events::KeyPressedEvent& e);
	};
}