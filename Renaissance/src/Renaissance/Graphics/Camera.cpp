#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Renaissance::Graphics
{
	using namespace Math;

	Camera::Camera()
		: mViewWidth(1280.0f), mViewHeight(720.0f), mNear(0.1f), mFar(100.0f)
	{
		UpdateProjection();
		UpdateView();
	}

	Camera::Camera(float viewportWidth, float viewportHeight, float nearClip, float farClip)
		: mViewWidth(viewportWidth), mViewHeight(viewportHeight), mNear(nearClip), mFar(farClip)
	{
		UpdateProjection();
		UpdateView();
	}
	
	Quat Camera::GetOrientation() const
	{
		return Quat(Vector3(-mPitch, -mYaw, -mRoll));
	}

	Vector3 Camera::GetUpVector() const
	{
		return glm::rotate(GetOrientation(), WorldUp);
	}

	Vector3 Camera::GetRightVector() const
	{
		return glm::rotate(GetOrientation(), WorldRight);
	}

	Vector3 Camera::GetForwardVector() const
	{
		return glm::rotate(GetOrientation(), WorldForward);
	}

	void Camera::UpdateProjection()
	{
		float aspectRatio = mViewWidth / mViewHeight;

		if (mIsOrthographic)
		{
			mProjection = glm::ortho(-aspectRatio * mOrthoScale, aspectRatio * mOrthoScale, -mOrthoScale, mOrthoScale, mNear, mFar);
		}
		else
		{
			mProjection = glm::perspective(glm::radians(mFov), aspectRatio, mNear, mFar);
		}
	}

	void Camera::UpdateView()
	{
		Quat orientation = GetOrientation();
		mView = glm::translate(IdentityMatrix, mLocation) * glm::toMat4(orientation);
		mView = glm::inverse(mView);
	}

	SharedPtr<Camera> Camera::MakePerspective(float viewportWidth, float viewportHeight, float fovDegrees /*= 60.0f*/, float nearClip /*= 0.1f*/, float farClip /*= 100.0f*/)
	{
		SharedPtr<Camera> camera = MakeShared<Camera>(viewportWidth, viewportHeight, nearClip, farClip);
		camera->SetPerspective();
		camera->SetFieldOfView(fovDegrees);
		return camera;
	}

	SharedPtr<Camera> Camera::MakeOrthographic(float viewportWidth, float viewportHeight, float orthoScale /*= 1.0f*/, float nearClip /*= -1.0f*/, float farClip /*= 1.0f*/)
	{
		SharedPtr<Camera> camera = MakeShared<Camera>(viewportWidth, viewportHeight, nearClip, farClip);
		camera->SetOrthographic();
		camera->SetOrthoScale(orthoScale);
		return camera;
	}
}