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
		EditorCameraController(const Graphics::Camera& camera);

		void OnEvent(Events::Event& e);

		void UpdateFirstPerson(Vector2 mouseDelta);
		void UpdateDrag(Vector2 mouseDelta);

		void IncreaseFoV();
		void DecreaseFoV();

		bool OnMouseScrolled(Events::MouseScrolledEvent& e);

		bool WantsConsumeMouseInput() const { return mConsumeMouseMoveInput; }

		const Graphics::Camera& GetCamera() const { return mCamera; }
		Matrix4 GetTransform() const { return glm::translate(IdentityMatrix, mLocation) * glm::toMat4(GetOrientation()); }
		Quat GetOrientation() const { return Quat(Vector3(-mPitch, -mYaw, -mRoll)); }
		Vector3 GetUpVector() const { return glm::rotate(GetOrientation(), WorldUp); }
		Vector3 GetRightVector() const { return glm::rotate(GetOrientation(), WorldRight); }
		Vector3 GetForwardVector() const { return glm::rotate(GetOrientation(), WorldForward); }
		void SetViewportSize(float width, float height) { mCamera.SetViewportSize(width, height); }

	private:
		Graphics::Camera mCamera;

		Vector3 mLocation = ZeroVector;
		float mPitch = 0.0f, mYaw = 0.0f, mRoll = 0.0f;

		float mCameraPanSpeed = 5.0f;
		float mCameraLookSpeed = 0.5f;
		bool mConsumeMouseMoveInput = false;
		Vector2 mInitialMousePosition = ZeroVector;		
	};
}