#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Math/Math.h"

namespace Renaissance::Graphics
{
	using namespace Math;

	class Camera
	{
	public:
		Camera();
		Camera(float viewportWidth, float viewportHeight, float fovDegrees, float nearClip, float farClip);
		Camera(float viewportWidth, float viewportHeight, float nearClip, float farClip);
		virtual ~Camera() = default;

		Quat GetOrientation() const;
		Vector3 GetUpVector() const;
		Vector3 GetRightVector() const;
		Vector3 GetForwardVector() const;

		void SetOrthographic(float nearClip = -1.0f, float farClip = 1.0f);
		void SetPerspective(float fovDegrees, float nearClip = 0.1f, float farClip = 100.0f);
		
		inline const Matrix4& GetProjectionMatrix() const { return mProjection; }
		inline const Matrix4& GetViewMatrix() const { return mView; }
		inline Matrix4 GetViewProjectionMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); } 
		inline const Vector3& GetLocation() const { return mLocation; }
		inline float GetPitch() const { return mPitch; }
		inline float GetYaw() const { return mYaw; }
		inline float GetViewportWidth() const { return mViewWidth; }
		inline float GetViewportHeight() const { return mViewHeight; }
		inline float GetFieldOfView() const { return mFov; }
		inline float GetNearClipping() const { return mNear; }
		inline float GetFarClipping() const { return mFar; }

		inline void SetLocation(const Vector3& location) { mLocation = location; UpdateView(); }
		inline void SetPitch(float pitch) { mPitch = pitch; UpdateView(); }
		inline void SetYaw(float yaw) { mYaw = yaw; UpdateView(); }
		inline void SetViewportSize(float width, float height) { mViewWidth = width; mViewHeight = height; UpdateProjection(); }
		inline void SetFieldOfView(float fovDegrees) { mFov = fovDegrees; UpdateProjection(); }
		inline void SetNearClipping(float nearClip) { mNear = nearClip; UpdateProjection(); }
		inline void SetFarClipping(float farClip) { mFar = farClip; UpdateProjection(); }

		static SharedPtr<Camera> MakePerspective(float viewportWidth, float viewportHeight, float fovDegrees = 60.0f, float nearClip = 0.1f, float farClip = 100.0f);
		static SharedPtr<Camera> MakeOrthographic(float viewportWidth, float viewportHeight, float nearClip = -1.0f, float farClip = 1.0f);

	private:
		void UpdateProjection();
		void UpdateView();

	private:
		Matrix4 mProjection = IdentityMatrix;
		Matrix4 mView = IdentityMatrix;
		Vector3 mLocation = ZeroVector;
		
		float mPitch = 0.0f, mYaw = 0.0f;
		float mViewWidth = 1280, mViewHeight = 720;
		float mFov;
		float mNear, mFar;

		bool isOrthographic = false;
	};
}