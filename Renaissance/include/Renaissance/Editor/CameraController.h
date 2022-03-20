#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Events/KeyEvent.h"
#include "Renaissance/Events/MouseEvent.h"
#include "Renaissance/Graphics/Camera.h"
#include "Renaissance/Math/Math.h"

namespace Renaissance
{
	using namespace Math;

	class CameraController
	{
	public:
		CameraController(SharedPtr<Graphics::Camera> camera);

		void OnUpdate(float deltaTime);

	private:
		SharedPtr<Graphics::Camera> mCamera = nullptr;
		float mCameraPanSpeed = 5.0f;
		float mCameraLookSpeed = 0.5f;
		bool mCameraRotationEnabled = false;
		Vector2 mInitialMousePosition = ZeroVector;

		bool OnMouseMoved(Events::MouseMovedEvent& e);
		bool OnMouseScrolled(Events::MouseScrolledEvent& e);
		bool IncreaseFoV(Events::KeyPressedEvent& e);
		bool DecreaseFoV(Events::KeyPressedEvent& e);
		bool OnRightMousePressed(Events::MouseButtonPressedEvent& e);
		bool OnRightMouseReleased(Events::MouseButtonReleasedEvent& e);
	};
}