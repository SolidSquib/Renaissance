#pragma once

#include "Renaissance.h"

namespace Renaissance
{
	class EditorLayer : public Layer
	{
	public:
		virtual void OnAttached() override;
		virtual void OnDetached() override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnEvent(Events::Event& e) override;

	private:
		SharedPtr<Graphics::Camera> mSceneCamera;
		SharedPtr<CameraController> mCameraController;
		SharedPtr<Graphics::Texture2D> mAwesomeFace;
		SharedPtr<Graphics::Texture2D> mGrass;
		SharedPtr<Graphics::Texture2D> mContainer;
		SharedPtr<Graphics::FrameBuffer> mSceneBuffer;

		bool mShowEditor = true;
	};
}