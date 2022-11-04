#include "FairegroundPCH.h"
#include "Faireground/Windows/PropertyEditorWindow.h"

#include "entt.hpp"
#include "imgui_internal.h"

#include "Faireground/EditorLayer.h"
#include "Faireground/Windows/PropertyEditorHelpers.h"

namespace Renaissance
{
	void PropertyEditorWindow::Open()
	{
		
	}

	void PropertyEditorWindow::Close()
	{
		
	}

	void PropertyEditorWindow::OnUIRender()
	{
		Entity selectedEntity = EditorLayer::GetSelectedEntity();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Property Editor", &mOpen);
		{
			if (selectedEntity)
			{
				PropertyEditor::DrawComponent(selectedEntity.GetComponent<IdentifierComponent>(), selectedEntity);
				PropertyEditor::DrawComponent(selectedEntity.GetComponent<TransformComponent>(), selectedEntity);

				if (selectedEntity.HasComponent<SpriteRendererComponent>())
					PropertyEditor::DrawComponent(selectedEntity.GetComponent<SpriteRendererComponent>(), selectedEntity);

				if (selectedEntity.HasComponent<CameraComponent>())
					PropertyEditor::DrawComponent(selectedEntity.GetComponent<CameraComponent>(), selectedEntity);

				if (selectedEntity.HasComponent<NativeScriptComponent>())
					PropertyEditor::DrawComponent(selectedEntity.GetComponent<NativeScriptComponent>(), selectedEntity);

				if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowWidth(), 20.0f)))
				{
					ImGui::OpenPopup("AddComponentMenu");
				}

				if (ImGui::BeginPopup("AddComponentMenu"))
				{
					if (ImGui::MenuItem("Add Sprite Renderer", nullptr, nullptr, !selectedEntity.HasComponent<SpriteRendererComponent>()))
					{
						selectedEntity.AddComponent<SpriteRendererComponent>();
					}

					if (ImGui::MenuItem("Add Camera", nullptr, nullptr, !selectedEntity.HasComponent<CameraComponent>()))
					{
						selectedEntity.AddComponent<CameraComponent>();
					}

					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void PropertyEditorWindow::OnUpdate(float deltaTime)
	{
		
	}
}
