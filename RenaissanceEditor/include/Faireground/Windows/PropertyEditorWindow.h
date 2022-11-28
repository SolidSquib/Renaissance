#pragma once

#include "Faireground/Windows/EditorWindow.h"
#include "Renaissance/Scene/Entity.h"
#include "Renaissance/Scene/Component.h"

namespace Renaissance
{
	class PropertyEditorWindow : public EditorWindow
	{
	public:
		PropertyEditorWindow() : EditorWindow("Property Editor") {}

		virtual void Open() override;
		virtual void Close() override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float deltaTime) override;

		virtual bool WantsToClose() const override { return !mOpen; }

		static String GetWindowClassNameStatic() { return "PropertyEditor"; }
		virtual String GetWindowClassName() const final { return PropertyEditorWindow::GetWindowClassNameStatic(); }

	private:
		bool mOpen = true;
	};
}