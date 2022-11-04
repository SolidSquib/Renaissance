#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Camera.h"

namespace Renaissance::Graphics
{
	using namespace Math;

	Camera::Camera()
		: mViewWidth(1280.0f), mViewHeight(720.0f), mNear(0.1f), mFar(100.0f)
	{
		UpdateProjection();
	}

	Camera::Camera(float viewportWidth, float viewportHeight, float nearClip, float farClip)
		: mViewWidth(viewportWidth), mViewHeight(viewportHeight), mNear(nearClip), mFar(farClip)
	{
		UpdateProjection();
	}

	Camera::Camera(const Camera& other)
		: mProjection(other.mProjection), mViewWidth(other.mViewWidth), mViewHeight(other.mViewHeight), 
		mFov(other.mFov), mOrthoScale(other.mOrthoScale), mNear(other.mNear), mFar(other.mFar), mProjectionMode(other.mProjectionMode)
	{ }

	void Camera::UpdateProjection()
	{
		float aspectRatio = mViewWidth / mViewHeight;

		switch (mProjectionMode)
		{
		case ProjectionMode::Perspective:
			mProjection = glm::perspective(glm::radians(mFov), aspectRatio, mNear, mFar);
			break;

		case ProjectionMode::Orthographic:
			mProjection = glm::ortho(-aspectRatio * mOrthoScale, aspectRatio * mOrthoScale, -mOrthoScale, mOrthoScale, mNear, mFar);
			break;
		}
	}

	Camera Camera::MakePerspective(float viewportWidth, float viewportHeight, float fovDegrees /*= 60.0f*/, float nearClip /*= 0.1f*/, float farClip /*= 100.0f*/)
	{
		Camera camera(viewportWidth, viewportHeight, nearClip, farClip);
		camera.SetPerspective();
		camera.SetFieldOfView(fovDegrees);
		return camera;
	}

	Camera Camera::MakeOrthographic(float viewportWidth, float viewportHeight, float orthoScale /*= 1.0f*/, float nearClip /*= -1.0f*/, float farClip /*= 1.0f*/)
	{
		Camera camera(viewportWidth, viewportHeight, nearClip, farClip);
		camera.SetOrthographic();
		camera.SetOrthoScale(orthoScale);
		return camera;
	}
}