#include "FairegroundPCH.h"
#include "Faireground/Windows/EditorViewportWindow.h"
#include "Faireground/EditorLayer.h"
#include "Faireground/Windows/PropertyEditorHelpers.h"

#include "ImGuizmo.h"


namespace Renaissance
{
	//////////////////////////////////////////////////////////////////////////
	//// EditorViewportWindow ////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	EditorViewportWindow::EditorViewportWindow(uint32_t viewportIndex)
		: EditorViewportWindow(viewportIndex, Graphics::Camera::MakePerspective(1280.0f, 720.0f))
	{ }

	EditorViewportWindow::EditorViewportWindow(uint32_t viewportIndex, const Graphics::Camera& camera)
		: mViewportCameraController(camera)
	{
		mViewportName = viewportIndex == 0 ? "Viewport" : "Viewport " + std::to_string(viewportIndex + 1);

		Graphics::FrameBuffer::Specification spec;
		spec.Width = (uint32_t)mCachedViewportSize.x;
		spec.Height = (uint32_t)mCachedViewportSize.y;

		Graphics::FrameBufferLayout sceneBufferLayout = {
			{ Graphics::FrameBufferAttachmentType::Color, true }
		};
		mViewportFrameBuffer = Graphics::FrameBuffer::Create(spec, sceneBufferLayout);
	}

	void EditorViewportWindow::OnUpdate(float deltaTime)
	{
		if (SharedPtr<Scene> activeScene = EditorLayer::GetActiveScene().lock())
		{
			activeScene->OnEditorUpdate(deltaTime);

			mViewportFrameBuffer->Bind();
			Graphics::RenderCommand::Clear(0);
			activeScene->OnRender(mViewportCameraController.GetCamera(), mViewportCameraController.GetTransform());
			mViewportFrameBuffer->Unbind();
		}
	}

	void EditorViewportWindow::OnUIRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(mViewportName.c_str(), &mOpen);
		{
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			Vector2 glmPanelSize = { viewportPanelSize.x, viewportPanelSize.y };
			bool viewportValidForRendering = viewportPanelSize.x > 0 && viewportPanelSize.y > 0;

			if (viewportValidForRendering)
			{
				if (mCachedViewportSize != glmPanelSize)
				{
					mCachedViewportSize = glmPanelSize;
					mViewportFrameBuffer->Resize((uint32_t)mCachedViewportSize.x, (uint32_t)mCachedViewportSize.y);
					mViewportCameraController.SetViewportSize(mCachedViewportSize.x, mCachedViewportSize.y);
				}

				static bool showBuffer = true;
				// NoMove flag prevents the window from moving unless dragging the titlebar, necessary to allow the use of 
				// gizmos in an undocked viewport.
				ImGui::BeginChild("Viewport FrameBuffer", viewportPanelSize, false, ImGuiWindowFlags_NoMove);
				{
					bool acceptMouseInput = ImGui::IsWindowHovered();
					ImVec2 currentMousePos = ImGui::GetMousePos();

					uint32_t sceneBufferColorId = mViewportFrameBuffer->GetAttachmentRendererId(Graphics::FrameBufferAttachmentType::Color);
					ImGui::Image((void*)(uint64_t)sceneBufferColorId, { mCachedViewportSize.x, mCachedViewportSize.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

					DrawViewportSettings();

					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && acceptMouseInput && mButtonFocus == ImGuiMouseButton_COUNT)
					{
						ImGui::SetWindowFocus();
						mButtonFocus = ImGuiMouseButton_Right;
						InputManager::DisableMouseCursor();
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle) && acceptMouseInput && mButtonFocus == ImGuiMouseButton_COUNT)
					{
						ImGui::SetWindowFocus();
						mButtonFocus = ImGuiMouseButton_Middle;
						InputManager::DisableMouseCursor();
					}
					else if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && mButtonFocus == ImGuiMouseButton_Right)
					{
						ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right, 0.5f);
						mViewportCameraController.UpdateFirstPerson({ mouseDelta.x , mouseDelta.y });
						ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
					}
					else if (ImGui::IsMouseDown(ImGuiMouseButton_Middle) && mButtonFocus == ImGuiMouseButton_Middle)
					{						
						ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle, 0.5f);
						mViewportCameraController.UpdateDrag({ mouseDelta.x , mouseDelta.y });
						ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
					}
					else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && mButtonFocus == ImGuiMouseButton_Right)
					{
						mButtonFocus = ImGuiMouseButton_COUNT;
						InputManager::EnableMouseCursor();
					}
					else if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle) && mButtonFocus == ImGuiMouseButton_Middle)
					{
						mButtonFocus = ImGuiMouseButton_COUNT;
						InputManager::EnableMouseCursor();
					}

					if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
					{
						if (ImGui::GetIO().MouseWheel > 0)
						{
							mViewportCameraController.DecreaseFoV();
						}
						else if (ImGui::GetIO().MouseWheel < 0)
						{
							mViewportCameraController.IncreaseFoV();
						}
					}

					// Render gizmos
					Entity selectedEntity = EditorLayer::GetSelectedEntity();
					if (selectedEntity && mGizmoManipulateOperation != -1)
					{
						ImGuizmo::SetOrthographic(mViewportCameraController.GetCamera().IsOrthographic());
						ImGuizmo::SetDrawlist();

						ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewportPanelSize.x, viewportPanelSize.y);
						
						Math::Matrix4 projectionMatrix = mViewportCameraController.GetCamera().GetProjectionMatrix();
						Math::Matrix4 viewMatrix = glm::inverse(mViewportCameraController.GetTransform());
						TransformComponent& transformComponent = selectedEntity.GetComponent<TransformComponent>();
						Math::Matrix4 entityTransform = transformComponent.GetTransform();

						float* snappingVar = mGizmoManipulateOperation == 1 ? &mGizmoRotationSnapping : &mGizmoTranslationSnapping;
						bool wantsSnapping = mGizmoManipulateOperation == 1 ? mGizmoEnableRotationSnapping : mGizmoEnableTranslationSnapping;

						ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), (ImGuizmo::OPERATION)mGizmoManipulateOperation, (ImGuizmo::MODE)mGizmoManipulateSpace, glm::value_ptr(entityTransform),
							nullptr, wantsSnapping ? snappingVar : nullptr);

						if (ImGuizmo::IsUsing() && acceptMouseInput)
						{
							Vector3 translation, rotation, scale;
							Math::DecomposeTransform(entityTransform, translation, rotation, scale);
							transformComponent.Location = translation;
							Math::Vector3 deltaRotation = rotation - transformComponent.Rotation;
							transformComponent.Rotation += deltaRotation;
							transformComponent.Scale = scale;
						}
					}
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorViewportWindow::DrawViewportSettings()
	{
		ImGui::SameLine(ImGui::GetWindowWidth() - 20.0f);

		if (ImGui::ArrowButton("ViewportSettingsButton", ImGuiDir_Down))
		{
			ImGui::OpenPopup("ViewportSettings");
		}

		if (ImGui::BeginPopup("ViewportSettings"))
		{
			ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, "Gizmo");
			ImGui::Separator();

			if (ImGui::Selectable("Disable Gizmo", mGizmoManipulateOperation == -1, ImGuiSelectableFlags_DontClosePopups))
				mGizmoManipulateOperation = -1;
			if (ImGui::Selectable("Translate", mGizmoManipulateOperation == 0, ImGuiSelectableFlags_DontClosePopups))
				mGizmoManipulateOperation = 0;
			if (ImGui::Selectable("Rotate", mGizmoManipulateOperation == 1, ImGuiSelectableFlags_DontClosePopups))
				mGizmoManipulateOperation = 1;
			if (ImGui::Selectable("Scale", mGizmoManipulateOperation == 2, ImGuiSelectableFlags_DontClosePopups))
				mGizmoManipulateOperation = 2;

			ImGui::Separator();

			if (ImGui::Selectable("Transform Local", mGizmoManipulateSpace == 0, ImGuiSelectableFlags_DontClosePopups))
				mGizmoManipulateSpace = 0;
			if (ImGui::Selectable("Transform World", mGizmoManipulateSpace == 1, ImGuiSelectableFlags_DontClosePopups))
				mGizmoManipulateSpace = 1;

			ImGui::Separator();			

			ImGui::Checkbox("Enable Translation Snapping", &mGizmoEnableTranslationSnapping);
			ImGui::DragFloat("Translation Snapping", &mGizmoTranslationSnapping, 0.1f, 0.0f, 100.0f, "%.2f");
			ImGui::Checkbox("Enable Rotation Snapping", &mGizmoEnableRotationSnapping);
			ImGui::DragFloat("Rotation Snapping", &mGizmoRotationSnapping, 0.1f, 0.0f, 100.0f, "%.2f");

			ImGui::Spacing();
			ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, "Editor Camera");
			ImGui::Separator();

			if (ImGui::Selectable("Perspective", !mViewportCameraController.GetCamera().IsOrthographic(), ImGuiSelectableFlags_DontClosePopups))
				mViewportCameraController.SetPerspective();
			if (ImGui::Selectable("Orthographic", mViewportCameraController.GetCamera().IsOrthographic(), ImGuiSelectableFlags_DontClosePopups))
				mViewportCameraController.SetOrthographic();

			ImGui::Separator();

			Vector3 cameraLocation = mViewportCameraController.GetLocation();
			Vector3 cameraRotation = mViewportCameraController.GetRotationVector();
			PropertyEditor::DrawVector3Control("Location", cameraLocation, 0.0f);
			PropertyEditor::DrawVector3Control("Rotation", cameraRotation, 0.0f);
			mViewportCameraController.SetLocation(cameraLocation);
			mViewportCameraController.SetRotation(cameraRotation);

			ImGui::EndPopup();
		}
	}
}