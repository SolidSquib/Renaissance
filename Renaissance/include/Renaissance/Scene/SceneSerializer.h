#pragma once

#include "Renaissance/Scene/Entity.h"
#include "Renaissance/Scene/Component.h"
#include "Renaissance/Scene/Scene.h"

#include <entt.hpp>
#include <yaml-cpp/yaml.h>
#include <tuple>
#include <queue>

namespace Renaissance
{	
	class Scene;

	class SceneSerializer
	{
	public:
		SceneSerializer(const SharedPtr<Scene>& scene);
		virtual ~SceneSerializer() = default;

		void SerializeText(const String& filepath);
		void SerializeBinary(const String& filepath);
		bool DeserializeText(const String& filepath);
		bool DeserializeBinary(const String& filepath);

	private:
		SharedPtr<Scene> mScene;
	};
// 
// 	enum class ESerializeMode { Text, Binary };
// 
// 	class SceneWriter
// 	{
// 	public:
// 		SceneWriter(const SharedPtr<Scene>& scene, Archive& archive)
// 			: mScene(scene), mArchive(archive)
// 		{ }
// 
// 		// serialize
// 		void operator()(entt::entity entity);
// 		void operator()(std::underlying_type_t<entt::entity> entity);
// 		/*void operator()(entt::entity entity, const IdentifierComponent& component);
// 		void operator()(entt::entity entity, const TagComponent& component);
// 		void operator()(entt::entity entity, const TransformComponent& component);
// 		void operator()(entt::entity entity, const SpriteRendererComponent& component);
// 		void operator()(entt::entity entity, const CameraComponent& component);*/
// 
// 		/*template<typename... Type>
// 		void operator()(const Type&...value)
// 		{
// 			(std::get<std::queue<Type>>(mArchive.mData).push(value), ...);
// 		}*/
// 
// 		template<typename T>
// 		void operator()(const entt::entity& entity, const T& component)
// 		{
// 			component.Serialize(mArchive);
// 		}		
// 
// 	private:
// 		SharedPtr<Scene> mScene;
// 		Archive& mArchive;
// 	};
// 
// 	class SceneReader
// 	{
// 	public:
// 		SceneReader(const SharedPtr<Scene>& scene, const Archive& archive)
// 			: mScene(scene), mArchive("archive")
// 		{ }
// 
// 		// deseriali
// 		void operator()(entt::entity& entity);
// 		void operator()(std::underlying_type_t<entt::entity>& entity);
// 		/*void operator()(entt::entity& entity, IdentifierComponent& component);
// 		void operator()(entt::entity& entity, TagComponent& component);
// 		void operator()(entt::entity& entity, TransformComponent& component);
// 		void operator()(entt::entity& entity, SpriteRendererComponent& component);
// 		void operator()(entt::entity& entity, CameraComponent& component);*/
// 
// 		template<typename T>
// 		void operator()(entt::entity& entity, T& component)
// 		{
// 			component.Deserialize(mArchive);
// 		}
// 
// 	private:
// 		SharedPtr<Scene> mScene;
// 		Archive mArchive; // TODO: If there's a way to avoid copying the whole archive, then great!
// 	};
}