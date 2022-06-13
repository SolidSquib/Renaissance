#include "FairegroundPCH.h"
#include "Faireground/Windows/SceneHierarchyWindow.h"

#include "Faireground/EditorLayer.h"

namespace Renaissance
{
	
	void SceneHierarchyWindow::OnUIRender()
	{
		ImGui::Begin("Scene Hierarchy", &mOpen);
		{
			if (SharedPtr<Scene> scene = mScene.lock())
			{
				scene->IterateEntities([](const Entity& entity, void* data) {

					Entity* selectedEntity = static_cast<Entity*>(data);

					const IdentifierComponent& id = entity.GetComponent<IdentifierComponent>();
					
					bool isOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, (*selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick, id.Name.c_str());
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						EditorLayer::OnSelectionChanged.Invoke(entity);
					}

					if (isOpen)
					{


						ImGui::TreePop();
					}

				}, &mSelectedEntity);
			}
		}
		ImGui::End();
	}

	void SceneHierarchyWindow::OnEntitySelectionChanged(const Entity& newSelection)
	{
		mSelectedEntity = newSelection;
	}

	void SceneHierarchyWindow::Open()
	{
		mOnSelectedEntityChangedDelegate = EditorLayer::OnSelectionChanged.Bind(*this, &SceneHierarchyWindow::OnEntitySelectionChanged);
	}

	void SceneHierarchyWindow::Close()
	{
		EditorLayer::OnSelectionChanged.Unbind(mOnSelectedEntityChangedDelegate);
	}
}