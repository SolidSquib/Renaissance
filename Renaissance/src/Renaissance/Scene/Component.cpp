#include "RenaissancePCH.h"
#include "Renaissance/Scene/Component.h"

namespace Renaissance
{
	void TransformComponent::Translate(const Math::Vector3& translation)
	{
		Location += translation;
	}

	void TransformComponent::Serialize(Archive& ar) const
	{
		REN_CORE_TRACE("Serialize TRANSFORM");
		ar << Location.x << Location.y << Location.z;
		ar << Rotation.x << Rotation.y << Rotation.z;
		ar << Scale.x << Scale.y << Scale.z;
	}

	void TransformComponent::Deserialize(Archive& ar)
	{
		REN_CORE_TRACE("Deserialize TRANSFORM");
		ar >> Location.x >> Location.y >> Location.z;
		ar >> Rotation.x >> Rotation.y >> Rotation.z;
		ar >> Scale.x >> Scale.y >> Scale.z;
	}

	void TagComponent::Serialize(Archive& ar) const
	{
		REN_CORE_TRACE("Serialize TAG");
		ar << Tag;
	}

	void TagComponent::Deserialize(Archive& ar)
	{
		REN_CORE_TRACE("Deserialize TAG");
		ar >> Tag;
	}

	void IdentifierComponent::Serialize(Archive& ar) const
	{
		REN_CORE_TRACE("Serialize ID");
		ar << (uint64_t)Guid;
	}

	void IdentifierComponent::Deserialize(Archive& ar)
	{
		REN_CORE_TRACE("Deserialize ID");
		uint64_t id;
		ar >> id;
		Guid = id;
	}

	void SpriteRendererComponent::Serialize(Archive& ar) const
	{
		REN_CORE_TRACE("Serialize SPRITE");
		ar << Color.r << Color.g << Color.b << Color.a;
		ar << Size.x << Size.y;
		ar << TilingFactor.x << TilingFactor.y;
		ar << Texture.MinCoord.x << Texture.MinCoord.y;
		ar << Texture.MaxCoord.x << Texture.MaxCoord.y;
		ar << (Texture.Texture ? Texture.Texture->GetPath().string() : "");
	}

	void SpriteRendererComponent::Deserialize(Archive& ar)
	{
		REN_CORE_TRACE("Deserialize SPRITE");
		ar >> Color.r >> Color.g >> Color.b >> Color.a;
		ar >> Size.x >> Size.y;
		ar >> TilingFactor.x >> TilingFactor.y;
		ar >> Texture.MinCoord.x >> Texture.MinCoord.y;
		ar >> Texture.MaxCoord.x >> Texture.MaxCoord.y;
		
		String filepath;
		ar >> filepath;

		if (!filepath.empty())
		{
			Texture.Texture = Graphics::TextureLibrary::GetGlobal().FindOrLoad<Graphics::Texture2D>(filepath);
		}
	}

	void CameraComponent::Serialize(Archive& ar) const
	{
		REN_CORE_TRACE("Serialize CAMERA");
		// TODO
	}

	void CameraComponent::Deserialize(Archive& ar)
	{
		REN_CORE_TRACE("Deserialize CAMERA");
	}

}
