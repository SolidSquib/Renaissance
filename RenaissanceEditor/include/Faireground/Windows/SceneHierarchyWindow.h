#pragma once

#include "Faireground/Windows/EditorWindow.h"
#include "Renaissance/Scene/Entity.h"
#include "Renaissance/Scene/Scene.h"

namespace Renaissance
{
	class SceneHierarchyWindow : public EditorWindow
	{
	public:
		SceneHierarchyWindow() = default;

		virtual void OnUIRender() override;
		virtual bool WantsToClose() const override { return !mOpen; }

		void Open() override;
		void Close() override;

	private:
		bool mOpen = true;
	};
}