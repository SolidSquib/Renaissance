#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Events/Event.h"

namespace Renaissance
{
	class Layer
	{
	public:
		Layer(const String& name = "Layer") : mDebugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttached() = 0;
		virtual void OnDetached() = 0;
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnUIRender() {}
		virtual void OnEvent(Events::Event& e) = 0;

		const String& GetName() const { return mDebugName; }

	protected:
		String mDebugName;
	};
}