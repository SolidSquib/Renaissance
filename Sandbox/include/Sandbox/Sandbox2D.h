#pragma once

#include "Renaissance.h"

namespace Sandbox
{
	using namespace Renaissance;

	class Sandbox2D : public Layer
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
	};
}