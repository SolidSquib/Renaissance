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

	Math::Vector3 Entity::GetLocation() const
	{
		const TransformComponent& transformComponent = GetComponent<TransformComponent>();
		return transformComponent.Location;
	}

	Math::Vector3 Entity::GetRotation() const
	{
		const TransformComponent& transformComponent = GetComponent<TransformComponent>();
		return transformComponent.Rotation;
	}

	Math::Vector3 Entity::GetScale() const
	{
		const TransformComponent& transformComponent = GetComponent<TransformComponent>();
		return transformComponent.Scale;
	}

	void Entity::SetLocation(const Math::Vector3& location)
	{
		TransformComponent& transformComponent = GetComponent<TransformComponent>();
		transformComponent.Location = location;
	}

	void Entity::SetRotation(const Math::Vector3& rotation)
	{
		TransformComponent& transformComponent = GetComponent<TransformComponent>();
		transformComponent.Rotation = rotation;
	}

	void Entity::SetScale(const Math::Vector3& scale)
	{
		TransformComponent& transformComponent = GetComponent<TransformComponent>();
		transformComponent.Scale = scale;
	}
}