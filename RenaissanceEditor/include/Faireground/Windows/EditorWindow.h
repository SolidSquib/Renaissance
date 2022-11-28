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
		EditorWindow(const String& name) : mName(name) { }
		virtual ~EditorWindow() = default;

		virtual void Open() { }
		virtual void Close() { }

		virtual void OnUpdate(float deltaTime) { }
		virtual void OnUIRender() = 0;

		virtual bool WantsInputFocus() const { return false; }
		virtual bool WantsToClose() const = 0;

		// For serialization
		virtual String GetWindowClassName() const = 0;
		String GetWindowName() const { return mName; };

	protected:
		String mName;
	};	
}