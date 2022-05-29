#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Events/KeyEvent.h"
#include "Renaissance/Events/MouseEvent.h"
#include "Renaissance/Graphics/Camera.h"
#include "Renaissance/Math/Math.h"

//#TODO move this class to the editor project.

namespace Renaissance
{
	using namespace Math;

	class EditorCameraController
	{
	public:
		EditorCameraController(SharedPtr<Graphics::Camera> camera);

		void OnEvent(Events::Event& e);

		void UpdateFirstPerson(Vector2 mouseDelta);
		void UpdateDrag(Vector2 mouseDelta);

		void IncreaseFoV();
		void DecreaseFoV();

		bool OnMouseScrolled(Events::MouseScrolledEvent& e);

		bool WantsConsumeMouseInput() const { return mConsumeMouseMoveInput; }

		WeakPtr<Graphics::Camera> GetCamera() const { return mCamera; }

	private:
		SharedPtr<Graphics::Camera> mCamera = nullptr;
		float mCameraPanSpeed = 5.0f;
		float mCameraLookSpeed = 0.5f;
		bool mConsumeMouseMoveInput = false;
		Vector2 mInitialMousePosition = ZeroVector;		
	};
}