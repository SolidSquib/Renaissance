#include "RenaissancePCH.h"
#include "Renaissance/Scene/Component.h"

namespace Renaissance
{
	void TransformComponent::Translate(const Math::Vector3& translation)
	{
		Transform = glm::translate(Transform, translation);
	}
}
