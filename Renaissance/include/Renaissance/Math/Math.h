#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// One day we can try to make our own math library maybe, or at least wrap this stuff so we don't need to include glm in all child projects.
namespace Renaissance
{
	namespace Math
	{
		using Vector2 = glm::vec2;
		using Vector3 = glm::vec3;
		using Vector4 = glm::vec4;

		using Matrix2 = glm::mat2;
		using Matrix3 = glm::mat3;
		using Matrix4 = glm::mat4;

		using Quat = glm::quat;

		static inline const Vector3 WorldUp = Vector3(0.0f, 1.0f, 0.0f);
		static inline const Vector3 WorldRight = Vector3(1.0f, 0.0f, 0.0f);
		static inline const Vector3 WorldForward = Vector3(0.0f, 0.0f, -1.0f);
		static inline const Vector3 ZeroVector = Vector3(0.0f, 0.0f, 0.0f);

		static inline const Matrix4 IdentityMatrix = Matrix4(1.0f);

		bool DecomposeTransform(const Matrix4& transform, Vector3& translation, Vector3& rotation, Vector3& scale);
	}
}