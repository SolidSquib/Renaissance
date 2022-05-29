#pragma once

#include "Renaissance.h"
#include "Faireground/Windows/EditorViewportWindow.h"

#include "imgui.h"

namespace Renaissance
{
	class EditorLayer : public Layer
	{
		static const uint32_t MaxViewports = 4;

	public:
		virtual void OnAttached() override;
		virtual void OnDetached() override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnEvent(Events::Event& e) override;

	private:
// 		SharedPtr<Graphics::Camera> mSceneCamera;
// 		SharedPtr<EditorCameraController> mCameraController;
// 		SharedPtr<Graphics::Texture2D> mAwesomeFace;
// 		SharedPtr<Graphics::Texture2D> mGrass;
// 		SharedPtr<Graphics::Texture2D> mContainer;
// 		SharedPtr<Graphics::FrameBuffer> mSceneBuffer;
// 		Vector2 mViewportSize;

		std::array<WeakPtr<EditorViewportWindow>, MaxViewports> mViewports;
		EditorWindowStack mWindowStack;

		bool mShowEditor = true;
	};
}