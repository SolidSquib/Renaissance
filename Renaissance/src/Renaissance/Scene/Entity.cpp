#include "RenaissancePCH.h"
#include "Renaissance/Scene/Entity.h"

namespace Renaissance
{
	Entity::Entity(const entt::entity& handle, Scene* parent)
		: mHandle(handle), mScene(parent)
	{
		REN_CORE_ASSERT(mScene, "Orphaned entities are not supported!");
	}

	Entity::~Entity()
	{

	}

	void Entity::SetLocation(const Math::Vector3& location)
	{
		TransformComponent& transformComponent = GetComponent<TransformComponent>();
		transformComponent.Transform[3][0] = location.x;
		transformComponent.Transform[3][1] = location.y;
		transformComponent.Transform[3][2] = location.z;
	}

	Math::Vector3 Entity::GetLocation() const
	{
		const TransformComponent& transformComponent = GetComponent<TransformComponent>();
		return Math::Vector3(
			transformComponent.Transform[3][0],
			transformComponent.Transform[3][1],
			transformComponent.Transform[3][2]
		);
	}
}