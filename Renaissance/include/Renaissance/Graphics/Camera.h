#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Math/Math.h"

namespace Renaissance::Graphics
{
	using namespace Math;

	class Camera
	{
		enum ProjectionMode {
			Perspective,
			Orthographic
		};

	public:
		Camera();
		Camera(float viewportWidth, float viewportHeight, float nearClip, float farClip);
		Camera(const Camera& other);
		virtual ~Camera() = default;

		inline void SetOrthographic() { mProjectionMode = ProjectionMode::Orthographic; UpdateProjection(); }
		inline void SetPerspective() { mProjectionMode = ProjectionMode::Perspective; UpdateProjection(); }
		
		inline const Matrix4& GetProjectionMatrix() const { return mProjection; }
		inline float GetViewportWidth() const { return mViewWidth; }
		inline float GetViewportHeight() const { return mViewHeight; }
		inline float GetFieldOfView() const { return mFov; }
		inline float GetOrthoScale() const { return mOrthoScale; }
		inline float GetNearClipping() const { return mNear; }
		inline float GetFarClipping() const { return mFar; }
		inline bool IsOrthographic() const { return mProjectionMode == ProjectionMode::Orthographic; }

		inline void SetViewportSize(float width, float height) { mViewWidth = width; mViewHeight = height; UpdateProjection(); }
		inline void SetFieldOfView(float fovDegrees) { mFov = fovDegrees; UpdateProjection(); }
		inline void SetOrthoScale(float orthoScale) { mOrthoScale = orthoScale; UpdateProjection(); }
		inline void SetNearClipping(float nearClip) { mNear = nearClip; UpdateProjection(); }
		inline void SetFarClipping(float farClip) { mFar = farClip; UpdateProjection(); }

		static Camera MakePerspective(float viewportWidth, float viewportHeight, float fovDegrees = 60.0f, float nearClip = 0.1f, float farClip = 100.0f);
		static Camera MakeOrthographic(float viewportWidth, float viewportHeight, float orthoScale = 1.0f, float nearClip = -1.0f, float farClip = 1.0f);

		void UpdateProjection();
	private:

	private:
 		Matrix4 mProjection = IdentityMatrix;
		
		float mViewWidth = 1280, mViewHeight = 720;
		float mFov = 60.0f;
		float mOrthoScale = 1.0f;
		float mNear, mFar;

		ProjectionMode mProjectionMode = ProjectionMode::Perspective;
	};
}