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
		SceneHierarchyWindow(const SharedPtr<Scene>& scene) 
			: mScene(scene)
		{ }

		virtual void OnUIRender() override;

		void SetScene(const SharedPtr<Scene>& scene) { mScene = scene; }
		void OnEntitySelectionChanged(const Entity& newSelection);

		virtual bool WantsToClose() const override { return !mOpen; }


		void Open() override;
		void Close() override;

	private:
		bool mOpen = true;

		Entity mSelectedEntity;
		WeakPtr<Scene> mScene;
		Connection<void(const Entity&)> mOnSelectedEntityChangedDelegate;
	};
}