#include "FairegroundPCH.h"
#include "Faireground/Windows/EditorViewportWindow.h"

namespace Renaissance
{
	//////////////////////////////////////////////////////////////////////////
	//// EditorViewportWindow ////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	EditorViewportWindow::EditorViewportWindow(const String& name)
		: EditorViewportWindow(name, Graphics::Camera::MakePerspective(mCachedViewportSize.x, mCachedViewportSize.y))
	{ }

	EditorViewportWindow::EditorViewportWindow(const String& name, const SharedPtr<Graphics::Camera>& camera)
		: mName(name), mViewportCameraController(MakeShared<EditorCameraController>(camera))
	{
		Graphics::FrameBuffer::Specification spec;
		spec.Width = (uint32_t)mCachedViewportSize.x;
		spec.Height = (uint32_t)mCachedViewportSize.y;

		Graphics::FrameBufferLayout sceneBufferLayout = {
			{ Graphics::FrameBufferAttachmentType::Color, true }
		};
		mViewportFrameBuffer = Graphics::FrameBuffer::Create(spec, sceneBufferLayout);
	}

	void EditorViewportWindow::OnDraw()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(GetName().c_str());
		{
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			Vector2 glmPanelSize = { viewportPanelSize.x, viewportPanelSize.y };
			if (mCachedViewportSize != glmPanelSize)
			{
				mCachedViewportSize = glmPanelSize;
				mViewportFrameBuffer->Resize((uint32_t)mCachedViewportSize.x, (uint32_t)mCachedViewportSize.y);
				mViewportCameraController->GetCamera().lock()->SetViewportSize(mCachedViewportSize.x, mCachedViewportSize.y);
			}

			static bool showBuffer = true;
			ImGui::BeginChild("Viewport FrameBuffer", viewportPanelSize);
			{
				bool acceptMouseInput = ImGui::IsWindowHovered();
				ImVec2 currentMousePos = ImGui::GetMousePos();

				uint32_t sceneBufferColorId = mViewportFrameBuffer->GetAttachmentRendererId(Graphics::FrameBufferAttachmentType::Color);
				ImGui::Image((void*)(uint64_t)sceneBufferColorId, { mCachedViewportSize.x, mCachedViewportSize.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
								
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && acceptMouseInput)
				{
					ImGui::SetWindowFocus();
					mLastMousePosition = { currentMousePos.x, currentMousePos.y };
					mFocused = true;
					InputManager::DisableMouseCursor();
				}
				else if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && mFocused)
				{
					Vector2 mouseDelta = Vector2(currentMousePos.x, currentMousePos.y) - mLastMousePosition;
					mLastMousePosition = { currentMousePos.x, currentMousePos.y };
					mViewportCameraController->UpdateFirstPerson(mouseDelta);
				}
				else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
				{
					mFocused = false;
					InputManager::EnableMouseCursor();
				}

				if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
				{
					if (ImGui::IsKeyPressed(KeyCode::PageDown, false))
					{
						mViewportCameraController->DecreaseFoV();
					}
					else if (ImGui::IsKeyPressed(KeyCode::PageUp, false))
					{
						mViewportCameraController->IncreaseFoV();
					}
				}
			}
			ImGui::EndChild();

		}
		ImGui::End();
		ImGui::PopStyleVar();

		Graphics::Renderer::Get().BeginScene(mViewportCameraController->GetCamera().lock());

		mViewportFrameBuffer->Bind();
		Graphics::RenderCommand::Clear(0);
		mScene->Draw();
		mViewportFrameBuffer->Unbind();

		Graphics::Renderer::Get().EndScene();
	}
}