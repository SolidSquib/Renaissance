#pragma once

#include <glm/glm.hpp>

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
	}
}