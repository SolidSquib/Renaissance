#include "FairegroundPCH.h"
#include "Faireground/EditorCameraController.h"

#include "Renaissance/Core/Application.h"
#include "Renaissance/Core/InputManager.h"

#include "imgui.h"

namespace Renaissance
{
	EditorCameraController::EditorCameraController(const Graphics::Camera& camera)
		: mCamera(camera)
	{
		auto [x, y] = InputManager::GetMousePosition();
		mInitialMousePosition = { x, y };

		mLocation.z = 1.0f;
	}

	void EditorCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(REN_BIND_EVENT(EditorCameraController::OnMouseScrolled));
	}

	void EditorCameraController::UpdateFirstPerson(Vector2 mouseDelta)
	{
		float deltaTime = Application::Get().DeltaTime();
		mYaw += (mouseDelta.x * deltaTime * mCameraLookSpeed);
		mPitch += (mouseDelta.y * deltaTime * mCameraLookSpeed);
		
		Vector3 inputVector = ZeroVector;

		if (ImGui::IsKeyDown(KeyCode::W))
		{
			inputVector.z += 1.0f;
		}
		if (ImGui::IsKeyDown(KeyCode::S))
		{
			inputVector.z -= 1.0f;
		}
		if (ImGui::IsKeyDown(KeyCode::A))
		{
			inputVector.x -= 1.0f;
		}
		if (ImGui::IsKeyDown(KeyCode::D))
		{
			inputVector.x += 1.0f;
		}
		if (ImGui::IsKeyDown(KeyCode::Q))
		{
			inputVector.y -= 1.0f;
		}
		if (ImGui::IsKeyDown(KeyCode::E))
		{
			inputVector.y += 1.0f;
		}

		if (inputVector != ZeroVector)
		{
			Vector3 normalInput = glm::normalize(inputVector);

			mLocation += (GetForwardVector() * normalInput.z * deltaTime * mCameraPanSpeed);
			mLocation += (GetRightVector() * normalInput.x * deltaTime * mCameraPanSpeed);
			mLocation += (WorldUp * normalInput.y * deltaTime * mCameraPanSpeed);
		}

	}
	
	void EditorCameraController::UpdateDrag(Vector2 mouseDelta)
	{

	}

	void EditorCameraController::IncreaseFoV()
	{
		const float step = 5.0f;
		const float orthoStep = 0.2f;
		mCamera.SetFieldOfView(mCamera.GetFieldOfView() + step);
		mCamera.SetOrthoScale(mCamera.GetOrthoScale() + orthoStep);
	}

	void EditorCameraController::DecreaseFoV()
	{
		const float step = 5.0f;
		const float orthoStep = 0.2f;
		mCamera.SetFieldOfView(mCamera.GetFieldOfView() - step);
		mCamera.SetOrthoScale(mCamera.GetOrthoScale() - orthoStep);
	}

	bool EditorCameraController::OnMouseScrolled(Events::MouseScrolledEvent& e)
	{
		// #TODO: gotta find a way to pass on scrolling input.

		const float step = 2.0f;
		const float minSpeed = 1.0f;
		const float maxSpeed = 100.0f;

		mCameraPanSpeed = glm::clamp<float>(mCameraPanSpeed + step * e.GetOffsetY(), minSpeed, maxSpeed);

		return true;
	}
}