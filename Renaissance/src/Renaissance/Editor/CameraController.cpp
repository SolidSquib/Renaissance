#include "RenaissancePCH.h"
#include "Renaissance/Editor/CameraController.h"

#include "Renaissance/Core/Application.h"
#include "Renaissance/Core/InputManager.h"

namespace Renaissance
{
	CameraController::CameraController(SharedPtr<Graphics::Camera> camera)
		: mCamera(camera)
	{
		InputManager::BindMouseMovedEventCallback(REN_BIND_EVENT(CameraController::OnMouseMoved));
		InputManager::BindMouseScrolledEventCallback(REN_BIND_EVENT(CameraController::OnMouseScrolled));
		InputManager::BindKeyPressedEventCallback(KeyCode::PageUp, REN_BIND_EVENT(CameraController::IncreaseFoV));
		InputManager::BindKeyPressedEventCallback(KeyCode::PageDown, REN_BIND_EVENT(CameraController::DecreaseFoV));
		InputManager::BindMouseButtonPressedEventCallback(MouseCode::Right, REN_BIND_EVENT(CameraController::OnRightMousePressed));
		InputManager::BindMouseButtonReleasedEventCallback(MouseCode::Right, REN_BIND_EVENT(CameraController::OnRightMouseReleased));

		auto [x, y] = InputManager::GetMousePosition();
		mInitialMousePosition = { x, y };
	}

	void CameraController::OnUpdate(float deltaTime)
	{
		Vector3 inputVector = ZeroVector;

		if (InputManager::IsKeyPressed(KeyCode::W))
		{
			inputVector.z += 1.0f;
		}
		if (InputManager::IsKeyPressed(KeyCode::S))
		{
			inputVector.z -= 1.0f;
		}
		if (InputManager::IsKeyPressed(KeyCode::A))
		{
			inputVector.x -= 1.0f;
		}
		if (InputManager::IsKeyPressed(KeyCode::D))
		{
			inputVector.x += 1.0f;
		}
		if (InputManager::IsKeyPressed(KeyCode::Q))
		{
			inputVector.y -= 1.0f;
		}
		if (InputManager::IsKeyPressed(KeyCode::E))
		{
			inputVector.y += 1.0f;
		}

		if (inputVector != ZeroVector)
		{
			Vector3 normalInput = glm::normalize(inputVector);

			Vector3 cameraLocation = mCamera->GetLocation();
			cameraLocation += (mCamera->GetForwardVector() * normalInput.z * deltaTime * mCameraPanSpeed);
			cameraLocation += (mCamera->GetRightVector() * normalInput.x * deltaTime * mCameraPanSpeed);
			cameraLocation += (WorldUp * normalInput.y * deltaTime * mCameraPanSpeed);

			mCamera->SetLocation(cameraLocation);
		}

		// Adjust Camera roll
		if (InputManager::IsKeyPressed(KeyCode::Left))
		{
			mCamera->SetRoll(mCamera->GetRoll() - (mCameraLookSpeed * deltaTime));
		}
		if (InputManager::IsKeyPressed(KeyCode::Right))
		{
			mCamera->SetRoll(mCamera->GetRoll() + (mCameraLookSpeed * deltaTime));
		}
	}

	bool CameraController::OnMouseMoved(Events::MouseMovedEvent& e)
	{
		float deltaX = e.GetMouseX() - mInitialMousePosition.x;
		float deltaY = e.GetMouseY() - mInitialMousePosition.y;
		mInitialMousePosition = { e.GetMouseX(), e.GetMouseY() };

		if (!mCameraRotationEnabled)
		{
			return false;
		}

		mCamera->SetYaw(mCamera->GetYaw() + (deltaX * Application::Get().DeltaTime() * mCameraLookSpeed));
		mCamera->SetPitch(mCamera->GetPitch() + (deltaY * Application::Get().DeltaTime() * mCameraLookSpeed));

		return true;
	}

	bool CameraController::OnMouseScrolled(Events::MouseScrolledEvent& e)
	{
		const float step = 2.0f;
		const float minSpeed = 1.0f;
		const float maxSpeed = 100.0f;

		mCameraPanSpeed = glm::clamp<float>(mCameraPanSpeed + step * e.GetOffsetY(), minSpeed, maxSpeed);

		return true;
	}

	bool CameraController::IncreaseFoV(Events::KeyPressedEvent& e)
	{
		const float step = 5.0f;
		const float orthoStep = 0.2f;
		mCamera->SetFieldOfView(mCamera->GetFieldOfView() + step);
		mCamera->SetOrthoScale(mCamera->GetOrthoScale() + orthoStep);
		return true;
	}

	bool CameraController::DecreaseFoV(Events::KeyPressedEvent& e)
	{
		const float step = 5.0f;
		const float orthoStep = 0.2f;
		mCamera->SetFieldOfView(mCamera->GetFieldOfView() - step);
		mCamera->SetOrthoScale(mCamera->GetOrthoScale() - orthoStep);
		return true;
	}

	bool CameraController::OnRightMousePressed(Events::MouseButtonPressedEvent& e)
	{
		mCameraRotationEnabled = true;
		InputManager::DisableMouseCursor();
		return true;
	}

	bool CameraController::OnRightMouseReleased(Events::MouseButtonReleasedEvent& e)
	{
		mCameraRotationEnabled = false;
		InputManager::EnableMouseCursor();
		return true;
	}
}