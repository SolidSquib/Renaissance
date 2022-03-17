#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Renaissance::Graphics
{
	using namespace Math;

	Camera::Camera()
		: mViewWidth(1280.0f), mViewHeight(720.0f), mFov(60.0f), mNear(0.1f), mFar(100.0f), isOrthographic(false)
	{
		UpdateProjection();
		UpdateView();
	}

	Camera::Camera(float viewportWidth, float viewportHeight, float fovDegrees, float nearClip, float farClip)
		: mViewWidth(viewportWidth), mViewHeight(viewportHeight), mFov(fovDegrees), mNear(nearClip), mFar(farClip), isOrthographic(false)
	{
		UpdateProjection();
		UpdateView();
	}

	Camera::Camera(float viewportWidth, float viewportHeight, float nearClip, float farClip)
		: mViewWidth(viewportWidth), mViewHeight(viewportHeight), mFov(0.0f), mNear(nearClip), mFar(farClip), isOrthographic(true)
	{
		UpdateProjection();
		UpdateView();
	}
	
	Quat Camera::GetOrientation() const
	{
		return Quat(Vector3(-mPitch, -mYaw, 0.0f));
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

	void Camera::SetOrthographic(float nearClip /*= -1.0f*/, float farClip /*= 1.0f*/)
	{
		isOrthographic = true;
		mNear = nearClip;
		mFar = farClip;

		UpdateProjection();
	}

	void Camera::SetPerspective(float fovDegrees, float nearClip /*= 0.1f*/, float farClip /*= 100.0f*/)
	{
		isOrthographic = false;
		mFov = fovDegrees;
		mNear = nearClip;
		mFar = farClip;

		UpdateProjection();
	}

	void Camera::UpdateProjection()
	{
		if (isOrthographic)
		{
			mProjection = glm::ortho(0.0f, mViewWidth, 0.0f, mViewHeight, mNear, mFar);
		}
		else
		{
			float aspectRatio = mViewWidth / mViewHeight;
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
		return MakeShared<Camera>(viewportWidth, viewportHeight, fovDegrees, nearClip, farClip);
	}

	SharedPtr<Camera> Camera::MakeOrthographic(float viewportWidth, float viewportHeight, float nearClip /*= -1.0f*/, float farClip /*= 1.0f*/)
	{

		return MakeShared<Camera>(viewportWidth, viewportHeight, nearClip, farClip);
	}
}