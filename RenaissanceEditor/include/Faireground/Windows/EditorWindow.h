#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/Layer.h"
#include "Renaissance/Graphics/FrameBuffer.h"

#include "imgui.h"

namespace Renaissance
{
	class EditorWindow
	{
	public:
		virtual ~EditorWindow() = default;

		virtual void Open() { }
		virtual void Close() { }

		virtual void OnUpdate(float deltaTime) { }
		virtual void OnUIRender() = 0;

		virtual bool WantsInputFocus() const { return false; }
		virtual bool WantsToClose() const = 0;
	};
}