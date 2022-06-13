#include "FairegroundPCH.h"
#include "Faireground/Windows/PropertyEditorWindow.h"

#include "entt.hpp"

#include "Faireground/EditorLayer.h"

namespace Renaissance
{
	void PropertyEditorWindow::Open()
	{
		mOnSelectedEntityChangedDelegate = EditorLayer::OnSelectionChanged.Bind(*this, &PropertyEditorWindow::OnEntitySelectionChanged);
	}

	void PropertyEditorWindow::Close()
	{
		EditorLayer::OnSelectionChanged.Unbind(mOnSelectedEntityChangedDelegate);
	}

	void PropertyEditorWindow::OnUIRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Property Editor", &mOpen);
		{
			if (mSelectedEntity)
			{
				if (mSelectedEntity.HasComponent<TransformComponent>())
					PropertyEditor::DrawTransform(mSelectedEntity.GetComponent<TransformComponent>());

				if (mSelectedEntity.HasComponent<SpriteRendererComponent>())
					PropertyEditor::DrawSpriteRenderer(mSelectedEntity.GetComponent<SpriteRendererComponent>());

				if (mSelectedEntity.HasComponent<CameraComponent>())
					PropertyEditor::DrawCamera(mSelectedEntity.GetComponent<CameraComponent>());

				if (mSelectedEntity.HasComponent<NativeScriptComponent>())
					PropertyEditor::DrawNativeScript(mSelectedEntity.GetComponent<NativeScriptComponent>());
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void PropertyEditorWindow::OnUpdate(float deltaTime)
	{
		
	}

	void PropertyEditorWindow::OnEntitySelectionChanged(const Entity& newSelection)
	{
		mSelectedEntity = newSelection;
	}

	void PropertyEditor::DrawTransform(TransformComponent& transformComponent)
	{
		ImGui::BeginChild("Transform Component");
		{
			float location[3] = { transformComponent.Location.x, transformComponent.Location.y, transformComponent.Location.z };
			float rotation[3] = { transformComponent.Rotation.x, transformComponent.Rotation.y, transformComponent.Rotation.z };
			float scale[3] = { transformComponent.Scale.x, transformComponent.Scale.y, transformComponent.Scale.z };
			
			ImGui::DragFloat3("Location", location);
			ImGui::DragFloat3("Rotation", rotation);
			ImGui::DragFloat3("Scale", scale);

			transformComponent.Location.x = location[0]; transformComponent.Location.y = location[1]; transformComponent.Location.z = location[2];
			transformComponent.Rotation.x = rotation[0]; transformComponent.Rotation.y = rotation[1]; transformComponent.Rotation.z = rotation[2];
			transformComponent.Scale.x = scale[0]; transformComponent.Scale.y = scale[1]; transformComponent.Scale.z = scale[2];
		}
		ImGui::EndChild();
	}

	void PropertyEditor::DrawSpriteRenderer(SpriteRendererComponent& transformComponent)
	{
		ImGui::BeginChild("Sprite Renderer Component");
		{

		}
		ImGui::EndChild();
	}

	void PropertyEditor::DrawCamera(CameraComponent& transformComponent)
	{
		ImGui::BeginChild("Camera Component");
		{

		}
		ImGui::EndChild();
	}

	void PropertyEditor::DrawNativeScript(NativeScriptComponent& transformComponent)
	{
		ImGui::BeginChild("Native Script Component");
		{

		}
		ImGui::EndChild();
	}

}
