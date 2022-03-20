#pragma once

#include "Renaissance/Core/Layer.h"
#include "Renaissance/Editor/CameraController.h"
#include "Renaissance/Graphics/Camera.h"
#include "Renaissance/Graphics/Shader.h"
#include "Renaissance/Graphics/VertexArray.h"

namespace Sandbox
{
	using namespace Renaissance;

	class ExampleLayer : public Layer
	{
	public:
		virtual void OnAttached() override;
		virtual void OnDetached() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnEvent(Events::Event& e) override { }

	private:
		SharedPtr<Graphics::Shader> mShader;
		SharedPtr<Graphics::VertexArray> mVertexArray;
		SharedPtr<Graphics::Camera> mSceneCamera;
		SharedPtr<CameraController> mCameraController;
	};
}