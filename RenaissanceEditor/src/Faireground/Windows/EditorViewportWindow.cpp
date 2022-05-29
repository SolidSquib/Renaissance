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

			bool showBuffer = true;
			ImGui::BeginChild("Viewport FrameBuffer", viewportPanelSize);
			{
				mUpdateCameraController = (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImGui::IsWindowHovered()) || mViewportCameraController->WantsConsumeMouseInput();
				uint32_t sceneBufferColorId = mViewportFrameBuffer->GetAttachmentRendererId(Graphics::FrameBufferAttachmentType::Color);
				ImGui::Image((void*)(uint64_t)sceneBufferColorId, { mCachedViewportSize.x, mCachedViewportSize.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			}
			ImGui::EndChild();

		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorViewportWindow::OnUpdate(float deltaTime)
	{
		if (mUpdateCameraController)
		{
			mViewportCameraController->OnUpdate(Application::Get().DeltaTime());
		}

		Graphics::Renderer::Get().BeginScene(mViewportCameraController->GetCamera().lock());

		mViewportFrameBuffer->Bind();
		Graphics::RenderCommand::Clear(0);
		{
			Graphics::SpriteBatch spriteBatch;
			spriteBatch.Draw(Vector2(0.0f, 0.0f), Vector2(1.0f), Vector4(0.3f, 1.0f, 0.4f, 1.0f));
			spriteBatch.Draw(Vector2(-0.2f, 0.4f), Vector2(0.5f), Vector4(0.1f, 0.4f, 0.9f, 1.0f));
			spriteBatch.Draw(Vector2(-0.5f, -0.2f), Vector2(0.2f), Vector4(0.2f, 0.8f, 0.4f, 1.0f));
			spriteBatch.Draw(Vector2(0.6f, 0.1f), Vector2(0.2f), Vector4(0.5f, 0.2f, 0.4f, 1.0f));
			spriteBatch.Draw(Vector2(0.6f, 0.1f), Vector2(0.2f), Vector4(1.0f, 0.2f, 0.4f, 1.0f));
		}
		mViewportFrameBuffer->Unbind();

		Graphics::Renderer::Get().EndScene();
	}

	void EditorViewportWindow::OnEvent(Events::Event& e)
	{
		if (mUpdateCameraController)
		{
			mViewportCameraController->OnEvent(e);
		}
	}
}