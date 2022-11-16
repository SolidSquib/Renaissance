#include "FairegroundPCH.h"
#include "Faireground/Windows/SceneHierarchyWindow.h"

#include "Faireground/EditorLayer.h"

namespace Renaissance
{
	
	void SceneHierarchyWindow::OnUIRender()
	{
		Entity deferredEntityToDelete;

		ImGui::Begin("Scene Hierarchy", &mOpen);
		{			
			if (SharedPtr<Scene> sceneLock = EditorLayer::GetActiveScene().lock())
			{
				sceneLock->IterateEntities([](Scene& scene, Entity entity, Entity selectedEntity) {
										
					const TagComponent& id = entity.GetComponent<TagComponent>();
					
					bool isOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, (selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick, id.Tag.c_str());
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					{
						EditorLayer::SetSelectedEntity(entity);
					}

					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete Entity"))
						{
							if (selectedEntity == entity)
							{
								EditorLayer::SetSelectedEntity({});
							}

							scene.DestroyEntity(entity);
						}

						ImGui::EndPopup();
					}

					if (isOpen)
					{


						ImGui::TreePop();
					}

				}, EditorLayer::GetSelectedEntity());
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				EditorLayer::SetSelectedEntity({});
			}

			// Right click in blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create Empty"))
				{
					if (SharedPtr<Scene> scene = EditorLayer::GetActiveScene().lock())
					{
						scene->CreateEntity("Empty");
					}
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();

		if (deferredEntityToDelete.IsValid())
		{
			if (SharedPtr<Scene> scene = EditorLayer::GetActiveScene().lock())
			{
				scene->DestroyEntity(deferredEntityToDelete);
				EditorLayer::SetSelectedEntity({});
			}
		}
	}

	void SceneHierarchyWindow::Open()
	{
		
	}

	void SceneHierarchyWindow::Close()
	{
		
	}
}