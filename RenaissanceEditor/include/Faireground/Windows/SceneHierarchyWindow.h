#pragma once

#include "Faireground/Windows/EditorWindow.h"
#include "Renaissance/Scene/Entity.h"
#include "Renaissance/Scene/Scene.h"

namespace Renaissance
{
	class SceneHierarchyWindow : public EditorWindow
	{
	public:
		SceneHierarchyWindow() : EditorWindow("Scene Heirarchy") {}

		virtual void OnUIRender() override;
		virtual bool WantsToClose() const override { return !mOpen; }

		void Open() override;
		void Close() override;

		static String GetWindowClassNameStatic() { return "SceneHeirarchy"; }
		virtual String GetWindowClassName() const final { return SceneHierarchyWindow::GetWindowClassNameStatic(); }

	private:
		bool mOpen = true;
	};
}