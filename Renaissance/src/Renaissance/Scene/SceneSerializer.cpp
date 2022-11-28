#include "RenaissancePCH.h"
#include "Renaissance/Scene/SceneSerializer.h"

namespace YAML
{
	template<>
	struct convert<Renaissance::Math::Vector2>
	{
		static Node encode(const Renaissance::Math::Vector2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, Renaissance::Math::Vector2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Renaissance::Math::Vector3>
	{
		static Node encode(const Renaissance::Math::Vector3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, Renaissance::Math::Vector3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Renaissance::Math::Vector4>
	{
		static Node encode(const Renaissance::Math::Vector4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, Renaissance::Math::Vector4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Renaissance
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const Math::Vector2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Math::Vector3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Math::Vector4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const SharedPtr<Scene>& scene)
		: mScene(scene)
	{
		REN_CORE_ASSERT(scene, "Serializer requires a valid Scene object!");
	}

	static void SerializeEntityText(YAML::Emitter& out, const Entity& entity)
	{
		REN_CORE_ASSERT(entity.HasComponent<IdentifierComponent>() && entity.HasComponent<TransformComponent>());

		out << YAML::BeginMap;
		out << YAML::Key << "ID" << YAML::Value << (uint64_t)entity.GetGuid(); 

		const TagComponent& identifierComponent = entity.GetComponent<TagComponent>();
		const TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

		out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;
		{
			out << YAML::Key << "Tag" << YAML::Value << identifierComponent.Tag;
		}
		out << YAML::EndMap;

		out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
		{
			out << YAML::Key << "Location" << YAML::Value << transformComponent.Location;
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;
		}
		out << YAML::EndMap;

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			const SpriteRendererComponent spriteComponent = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
			{
				out << YAML::Key << "Tint" << YAML::Value << spriteComponent.Color;
				out << YAML::Key << "Size" << YAML::Value << spriteComponent.Size;
				out << YAML::Key << "TilingFactor" << YAML::Value << spriteComponent.TilingFactor;
				out << YAML::Key << "TexturePath" << YAML::Value << (spriteComponent.Texture.Texture ? spriteComponent.Texture.Texture->GetPath().string() : "");
				out << YAML::Key << "MinCoord" << YAML::Value << spriteComponent.Texture.MinCoord;
				out << YAML::Key << "MaxCoord" << YAML::Value << spriteComponent.Texture.MaxCoord;
			}
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			const CameraComponent cameraComponent = entity.GetComponent<CameraComponent>();

			out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;
			{
				out << YAML::Key << "ViewportSize" << YAML::Value << Math::Vector2(cameraComponent.GetViewportWidth(), cameraComponent.GetViewportHeight());
				out << YAML::Key << "NearClipping" << YAML::Value << cameraComponent.GetNearClipping();
				out << YAML::Key << "FarClipping" << YAML::Value << cameraComponent.GetFarClipping();
				out << YAML::Key << "IsOrthographic" << YAML::Value << cameraComponent.IsOrthographic();
				out << YAML::Key << "OrthoScale" << YAML::Value << cameraComponent.GetOrthoScale();
				out << YAML::Key << "FieldOfView" << YAML::Value << cameraComponent.GetFieldOfView();
			}
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeText(const String& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Sample_Text";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		mScene->mRegistry.each([&](auto entityId) {

			Entity entity = { entityId, mScene.get() };
			if (!entity.IsValid())
				return;

			SerializeEntityText(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const String& filepath)
	{

	}

	bool SceneSerializer::DeserializeText(const String& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		String sceneName = data["Scene"].as<String>();
		REN_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto& entity : entities)
			{
				GUID guid = entity["ID"].as<uint64_t>();

				// TagComponent
				auto tagComponent = entity["TagComponent"];
				String tag = tagComponent["Tag"].as<String>();
				
				Entity& deserializedEntity = mScene->CreateEntity(tag, &guid);

				// TransformComponent
				auto transformComponentNode = entity["TransformComponent"];				
				TransformComponent& transformComponent = deserializedEntity.GetComponent<TransformComponent>();
				transformComponent.Location = transformComponentNode["Location"].as<Math::Vector3>();
				transformComponent.Rotation = transformComponentNode["Rotation"].as<Math::Vector3>();
				transformComponent.Scale = transformComponentNode["Scale"].as<Math::Vector3>();

				// SpriteRendererComponent
				if (auto spriteComponentNode = entity["SpriteRendererComponent"])
				{
					SpriteRendererComponent& spriteComponent = deserializedEntity.AddComponent<SpriteRendererComponent>();
					spriteComponent.Color = spriteComponentNode["Tint"].as<Math::Vector4>();
					spriteComponent.Size = spriteComponentNode["Size"].as<Math::Vector2>();
					spriteComponent.TilingFactor = spriteComponentNode["TilingFactor"].as<Math::Vector2>();
					String filepath = spriteComponentNode["TexturePath"].as<String>();
					if (!filepath.empty() && !spriteComponentNode["TexturePath"].IsNull())
					{
						spriteComponent.Texture.Texture = Graphics::TextureLibrary::GetGlobal().FindOrLoad<Graphics::Texture2D>(filepath);
					}					
					spriteComponent.Texture.MinCoord = spriteComponentNode["MinCoord"].as<Math::Vector2>();
					spriteComponent.Texture.MaxCoord = spriteComponentNode["MaxCoord"].as<Math::Vector2>();
				}

				// CameraComponent
				if (auto cameraComponentNode = entity["CameraComponent"])
				{
					CameraComponent& cameraComponent = deserializedEntity.AddComponent<CameraComponent>();
					Math::Vector2 viewportSize = cameraComponentNode["ViewportSize"].as<Math::Vector2>();
					cameraComponent.SetViewportSize(viewportSize.x, viewportSize.y);
					cameraComponent.SetNearClipping(cameraComponentNode["NearClipping"].as<float>());
					cameraComponent.SetFarClipping(cameraComponentNode["FarClipping"].as<float>());
					if (cameraComponentNode["IsOrthographic"].as<bool>())
					{
						cameraComponent.SetOrthographic();
					}
					else
					{
						cameraComponent.SetPerspective();
					}
					cameraComponent.SetOrthoScale(cameraComponentNode["OrthoScale"].as<float>());
					cameraComponent.SetFieldOfView(cameraComponentNode["FieldOfView"].as<float>());
				}

				// NativeScriptComponent
				if (auto nativeScriptComponentNode = entity["NativeScriptComponent"])
				{
					// #TODO: figure out how to dynamically determine native script components from text...
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const String& filepath)
	{
		return false;
	}

	// serialization
// 	void SceneWriter::operator()(entt::entity entity)
// 	{
// 		REN_CORE_TRACE("Serialize ENTITY");
// 		/*mArchive << entity;*/
// 	}
// 	
// 	void SceneWriter::operator()(std::underlying_type_t<entt::entity> entity)
// 	{
// 		REN_CORE_TRACE("Serialize ENTITY INNER");
// 	/*	mArchive << entity;*/
// 	}
// 
// 	void SceneReader::operator()(entt::entity& entity)
// 	{
// 		REN_CORE_TRACE("Deserialize ENTITY");
// 	/*	mArchive >> entity;*/
// 	}
// 
// 	void SceneReader::operator()(std::underlying_type_t<entt::entity>& entity)
// 	{
// 		REN_CORE_TRACE("Deserialize ENTITY INNER");
// 		/*mArchive >> entity;*/
// 	}
}