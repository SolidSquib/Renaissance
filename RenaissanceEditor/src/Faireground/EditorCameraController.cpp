#include "FairegroundPCH.h"
#include "Faireground/EditorCameraController.h"

#include "Renaissance/Core/Application.h"
#include "Renaissance/Core/InputManager.h"

namespace Renaissance
{
	EditorCameraController::EditorCameraController(SharedPtr<Graphics::Camera> camera)
		: mCamera(camera)
	{
		auto [x, y] = InputManager::GetMousePosition();
		mInitialMousePosition = { x, y };
	}

	void EditorCameraController::OnUpdate(float deltaTime)
	{
		auto [x, y] = InputManager::GetMousePosition();
		float deltaX = x - mInitialMousePosition.x;
		float deltaY = y - mInitialMousePosition.y;		
		mInitialMousePosition = { x, y };

		//REN_CORE_WARN("Mouse {0}, {1}", x, y);

		if (!mConsumeMouseMoveInput)
			return;

		// FPS camera functionality
		if (InputManager::IsMouseButtonPressed(MouseCode::Right))
		{
			mCamera->SetYaw(mCamera->GetYaw() + (deltaX * deltaTime * mCameraLookSpeed));
			mCamera->SetPitch(mCamera->GetPitch() + (deltaY * deltaTime * mCameraLookSpeed));

			//////////////////////////////////////////////////////////////////////////

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
		}

		// Drag functionality
		else if (InputManager::IsMouseButtonPressed(MouseCode::Left))
		{
			
		}
	}

	void EditorCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(REN_BIND_EVENT(EditorCameraController::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(REN_BIND_EVENT(EditorCameraController::OnMouseButtonDown));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(REN_BIND_EVENT(EditorCameraController::OnMouseButtonUp));
		dispatcher.Dispatch<KeyPressedEvent>(REN_BIND_EVENT(EditorCameraController::OnKeyPressed));
	}

	bool EditorCameraController::OnMouseScrolled(Events::MouseScrolledEvent& e)
	{
		const float step = 2.0f;
		const float minSpeed = 1.0f;
		const float maxSpeed = 100.0f;

		mCameraPanSpeed = glm::clamp<float>(mCameraPanSpeed + step * e.GetOffsetY(), minSpeed, maxSpeed);

		return true;
	}

	bool EditorCameraController::OnMouseButtonDown(Events::MouseButtonPressedEvent& e)
	{
		if (e.GetButtonCode() == MouseCode::Left || e.GetButtonCode() == MouseCode::Right)
		{
			mConsumeMouseMoveInput = true;
			InputManager::DisableMouseCursor();
			return true;
		}

		return false;
	}

	bool EditorCameraController::OnMouseButtonUp(Events::MouseButtonReleasedEvent& e)
	{
		if (e.GetButtonCode() == MouseCode::Left || e.GetButtonCode() == MouseCode::Right)
		{
			mConsumeMouseMoveInput = false;
			InputManager::EnableMouseCursor();
			return true;
		}

		return false;
	}

	bool EditorCameraController::OnKeyPressed(Events::KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == KeyCode::PageUp)
		{
			const float step = 5.0f;
			const float orthoStep = 0.2f;
			mCamera->SetFieldOfView(mCamera->GetFieldOfView() + step);
			mCamera->SetOrthoScale(mCamera->GetOrthoScale() + orthoStep);
			return true;
		}
		else if (e.GetKeyCode() == KeyCode::PageDown)
		{
			const float step = 5.0f;
			const float orthoStep = 0.2f;
			mCamera->SetFieldOfView(mCamera->GetFieldOfView() - step);
			mCamera->SetOrthoScale(mCamera->GetOrthoScale() - orthoStep);
			return true;
		}

		return false;
	}
}