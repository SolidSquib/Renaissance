#include "RenaissancePCH.h"
#include "Renaissance/Scene/Component.h"

namespace Renaissance
{
	void TransformComponent::Translate(const Math::Vector3& translation)
	{
		Location += translation;
	}

	//DEFINE_COMPONENT_META(TagComponent);
	//DEFINE_COMPONENT_META(IdentifierComponent);
	//DEFINE_COMPONENT_META(TransformComponent);
	//DEFINE_COMPONENT_META(SpriteRendererComponent);
	//DEFINE_COMPONENT_META(CameraComponent);
}
