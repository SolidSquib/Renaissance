#pragma once

#include "Renaissance.h"
#include "Faireground/Windows/EditorWindow.h"

namespace Renaissance
{
	class EditorToolbarWindow : public EditorWindow
	{
	public:
		EditorToolbarWindow() : EditorWindow("Toolbar") {}

		virtual void OnUIRender() final;

		virtual bool WantsToClose() const final { return !mOpen; }

		static String GetWindowClassNameStatic() { return "EditorToolbar"; }
		virtual String GetWindowClassName() const final { return EditorToolbarWindow::GetWindowClassNameStatic(); }

	private:
		bool mOpen = true;

		float mButtonSize = 50.0f;
		float mButtonPadding = 14.0f;

		String savedScene;
	};
}