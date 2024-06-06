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
		: EditorWindow(viewportIndex == 0 ? "Viewport" : "Viewport " + std::to_string(viewportIndex + 1)),
		mIndex(viewportIndex),
		mViewportCameraController(camera)
	{
		Graphics::FrameBuffer::Specification spec;
		spec.Width = (uint32_t)mCachedViewportSize.x;
		spec.Height = (uint32_t)mCachedViewportSize.y;
		spec.Samples = 1;
		spec.Attachments = { Graphics::FrameBufferTextureFormat::RGBA8, Graphics::FrameBufferTextureFormat::RED_INTEGER_UNSIGNED, Graphics::FrameBufferTextureFormat::Depth };

		mViewportFrameBuffer = Graphics::FrameBuffer::Create(spec);
	}

	void EditorViewportWindow::OnUpdate(float deltaTime)
	{
		if (SharedPtr<Scene> activeScene = EditorLayer::GetActiveScene().lock())
		{
			mViewportFrameBuffer->Bind();
			Graphics::RenderCommand::Clear(0);
			mViewportFrameBuffer->ClearAttachment(1, { 0.0f,0.0f,0.0f,0.0f });

			if (EditorLayer::GetUpdateState() == EditorLayer::EEditorUpdateMode::Play)
			{
				// default render path will use the scene's main camera
				activeScene->OnRender();
			}
			else
			{
				activeScene->OnRender(mViewportCameraController.GetCamera(), mViewportCameraController.GetTransform());
			}

			mViewportFrameBuffer->Unbind();
		}
	}

	void EditorViewportWindow::OnUIRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(mName.c_str(), &mOpen);
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

				// NoMove flag prevents the window from moving unless dragging the titlebar, necessary to allow the use of 
				// gizmos in an undocked viewport.
				ImGui::BeginChild("Viewport FrameBuffer", viewportPanelSize, false, ImGuiWindowFlags_NoMove);
				{
					ImVec2 viewportOffset = ImGui::GetCursorPos();					

					uint32_t sceneBufferColorId = mViewportFrameBuffer->GetColorAttachmentRendererId(0);
					ImGui::Image((void*)(uint64_t)sceneBufferColorId, { mCachedViewportSize.x, mCachedViewportSize.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload(DRAG_CONTEXT_PATH_SCENE))
						{
							std::filesystem::path* filepath = reinterpret_cast<std::filesystem::path*>(Payload->Data);
							EditorLayer::OpenScene(*filepath);
						}

						ImGui::EndDragDropTarget();
					}

					ImVec2 minBound = ImGui::GetWindowPos();
					minBound.x -= viewportOffset.x;
					minBound.y -= viewportOffset.y;

					ImVec2 maxBound = { minBound.x + viewportPanelSize.x, minBound.y + viewportPanelSize.y };
					mViewportBounds[0] = { minBound.x, minBound.y };
					mViewportBounds[1] = { maxBound.x, maxBound.y };

					ImVec2 currentMousePos = ImGui::GetMousePos();
					currentMousePos.x -= mViewportBounds[0].x;
					currentMousePos.y -= mViewportBounds[0].y;
					Vector2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
					
					const bool areSettingsOpen = DrawViewportSettings();
					const bool acceptMouseInput = ImGui::IsWindowHovered() && !areSettingsOpen;

					SharedPtr<Scene> activeScene = EditorLayer::GetActiveScene().lock();
					if (activeScene)
					{
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

						if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && acceptMouseInput)
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

						ImGuizmo::SetOrthographic(mViewportCameraController.GetCamera().IsOrthographic());
						ImGuizmo::SetDrawlist();
						ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, viewportPanelSize.x, viewportPanelSize.y);

						Math::Matrix4 projectionMatrix = mViewportCameraController.GetCamera().GetProjectionMatrix();
						Math::Matrix4 viewMatrix = glm::inverse(mViewportCameraController.GetTransform());

						// Render gizmos
						Entity selectedEntity = EditorLayer::GetSelectedEntity();
						if (selectedEntity && mGizmoManipulateOperation != -1)
						{
							TransformComponent& transformComponent = selectedEntity.GetComponent<TransformComponent>();
							Math::Matrix4 entityTransform = transformComponent.GetTransform();

							float* snappingVar = mGizmoManipulateOperation == 1 ? &mGizmoRotationSnapping : &mGizmoTranslationSnapping;
							bool wantsSnapping = mGizmoManipulateOperation == 1 ? mGizmoEnableRotationSnapping : mGizmoEnableTranslationSnapping;

							Matrix4 delta;
							if (ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), (ImGuizmo::OPERATION)mGizmoManipulateOperation, (ImGuizmo::MODE)mGizmoManipulateSpace, glm::value_ptr(entityTransform),
								glm::value_ptr(delta), wantsSnapping ? snappingVar : nullptr))
							{
								// Something's up with how the delta computes in ortho projection mode, so I gotta take the numerical anomaly into account before applying
								// any transformations.
								if (mFirstFrame)
								{
									Math::DecomposeTransform(delta, mCachedDelta_Translation, mCachedDelta_Rotation, mCachedDelta_Scale);
									mFirstFrame = false;
								}
								else if (ImGuizmo::IsUsing() && acceptMouseInput)
								{
									Vector3 translation, rotation, scale;
									Math::DecomposeTransform(delta, translation, rotation, scale);
									transformComponent.Location += (translation - mCachedDelta_Translation);
									transformComponent.Rotation += (rotation - mCachedDelta_Rotation);
									transformComponent.Scale += (scale - mCachedDelta_Scale);
								}								
							}
							else if (!ImGuizmo::IsUsing())
							{
								mFirstFrame = true;
							}
						}

						if (acceptMouseInput && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing())
						{
							mViewportFrameBuffer->Bind();
							int entityId = mViewportFrameBuffer->ReadPixel(1, (uint32_t)currentMousePos.x, (uint32_t)(viewportSize.y - currentMousePos.y));
							mViewportFrameBuffer->Unbind();

							EditorLayer::SetSelectedEntity(Entity((entt::entity)(entityId - 1), activeScene.get()));
						}

						// view rotation widget and camera update
						ImVec2 widgetLocation = ImVec2(ImGui::GetWindowPos().x + viewportPanelSize.x - 100.f, ImGui::GetWindowPos().y);
						ImGuizmo::ViewManipulate(glm::value_ptr(viewMatrix), -100, widgetLocation, ImVec2(100.f, 100.f), 0);
					}					
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	bool EditorViewportWindow::DrawViewportSettings()
	{
		bool wasOpened = false;
		
		ImVec2 textSize = ImGui::CalcTextSize(ICON_FA_ELLIPSIS_H);
		ImGui::SameLine(ImGui::GetWindowWidth() - textSize.x - (ImGui::GetStyle().FramePadding.x * 2.0f));

		if (ImGui::Button(ICON_FA_ELLIPSIS_H))
		{
			wasOpened = true;
			ImGui::OpenPopup("ViewportSettings");			
		}

		if (ImGui::BeginPopup("ViewportSettings"))
		{
			wasOpened = true;
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
			Vector3 cameraRotation = glm::degrees(mViewportCameraController.GetRotationVector());
			PropertyEditor::DrawVector3Control("Location", cameraLocation, 0.0f);
			PropertyEditor::DrawVector3Control("Rotation", cameraRotation, 0.0f);
			mViewportCameraController.SetLocation(cameraLocation);
			mViewportCameraController.SetRotation(glm::radians(cameraRotation));

			ImGui::EndPopup();
		}

		return wasOpened;
	}
}