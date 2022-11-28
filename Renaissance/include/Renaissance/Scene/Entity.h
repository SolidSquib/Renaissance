#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Scene/Scene.h"

namespace entt
{
	template<class Archive>
	void serialize(Archive& archive, meta_any& any)
	{
		any.invoke("serialize"_hs, &archive);
	}
}

namespace Renaissance
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(const entt::entity& entityId, Scene* scene);
		virtual ~Entity();

		bool IsValid() const { return mHandle != entt::null && mScene != nullptr; }
		GUID GetGuid() const;

		Math::Vector3 GetLocation() const;
		Math::Vector3 GetRotation() const;
		Math::Vector3 GetScale() const;
		void SetLocation(const Math::Vector3& location);
		void SetRotation(const Math::Vector3& rotation);
		void SetScale(const Math::Vector3& scale);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			REN_CORE_ASSERT(!HasComponent<T>(), "Entity already has a component of this type.");
			return mScene->mRegistry.emplace<T>(mHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			REN_CORE_ASSERT(HasComponent<T>(), "Component does not exist");
			mScene->mRegistry.remove<T>(mHandle);
		}

		template<typename T>
		const T& GetComponent() const
		{
			REN_CORE_ASSERT(HasComponent<T>(), "Entity has no component of this type.");
			return mScene->mRegistry.get<T>(mHandle);
		}

		template<typename T> 
		T& GetComponent()
		{
			REN_CORE_ASSERT(HasComponent<T>(), "Entity has no component of this type.");
			return mScene->mRegistry.get<T>(mHandle);
		}

		template<typename T> 
		bool HasComponent() const
		{
			REN_CORE_ASSERT(mScene, "Scene is not valid!");
			return mScene->mRegistry.all_of<T>(mHandle);
		}

		explicit operator const entt::entity& () const { return mHandle; }
		explicit operator uint32_t () const { return (uint32_t)mHandle; }

		operator const bool() const { return IsValid(); }

		bool operator!=(const Entity& other) const
		{
			return mHandle != other.mHandle || mScene != other.mScene;
		}

		bool operator==(const Entity& other) const
		{
			return !(*this != other);
		}

		template<class Archive>
		void save(Archive& ar) const
		{
			std::vector<entt::id_type> componentTypes;
			for (auto&& curr : mScene->mRegistry.storage())
			{
				if (auto& storage = curr.second; storage.contains(mHandle))
				{
					componentTypes.push_back(curr.first);					
				}
			}

			ar(cereal::make_nvp("ComponentTypes", componentTypes));

			for (auto& type : componentTypes)
			{
				const auto metaType = entt::resolve(type);
				const auto any = metaType.func("get"_hs).invoke({}, &mScene->mRegistry, mHandle);
				const auto componentName = metaType.prop("name"_hs).value().cast<String>();
				ar(cereal::make_nvp(componentName, any));
			}
		}

		template<class Archive>
		void load(Archive& ar)
		{
			std::vector<entt::id_type> componentTypes;
			ar(componentTypes);
			
			REN_CORE_ASSERT(IsValid());
			
			for (auto& type : componentTypes)
			{
				const auto metaType = entt::resolve(type);
				entt::hashed_string id = metaType.prop("id"_hs).value().cast<entt::hashed_string>();
				entt::meta_any component;

				if (id == "TagComponent"_hs || id == "IdentifierComponent"_hs || id == "TransformComponent"_hs)
				{
					component = metaType.func("get"_hs).invoke({}, &mScene->mRegistry, mHandle);
				}
				else
				{
					component = metaType.func("add"_hs).invoke({}, &mScene->mRegistry, mHandle);
				}

				ar(component);
			}
		}

	private:
		entt::entity mHandle = entt::null;
		Scene* mScene = nullptr;
	};

	template<class Ar>
	void serialize(Ar& archive, entt::entity& entity)
	{
		archive((uint32_t)entity);
	}

	class ScriptableEntity
	{
	public:
		ScriptableEntity() = default;
		virtual ~ScriptableEntity() = default;
		
		Math::Vector3 GetLocation() const { return mEntity.GetLocation(); }
		Math::Vector3 GetRotation() const { return mEntity.GetRotation(); }
		Math::Vector3 GetScale() const { return mEntity.GetScale(); }
		void SetLocation(const Math::Vector3& location) { mEntity.SetLocation(location); }
		void SetRotation(const Math::Vector3& rotation) { mEntity.SetRotation(rotation); }
		void SetScale(const Math::Vector3& scale) { mEntity.SetScale(scale); }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) { return mEntity.AddComponent<T>(std::forward<Args>(args)...) }

		template<typename T>
		void RemoveComponent() { mEntity.RemoveComponent<T>(); }

		template<typename T>
		const T& GetComponent() const { return mEntity.GetComponent<T>(); }

		template<typename T>
		T& GetComponent() { return mEntity.GetComponent<T>(); }

		template<typename T>
		bool HasComponent() const { return mEntity.HasComponent<T>(); }

	private:
		Entity mEntity;

		friend class Scene;
		friend class SceneSerializer;
	};

	class EntityLoader
	{
	public:
		EntityLoader(Scene* scene) : mScene(scene) {}

		template<class Archive>
		void save(Archive& ar) const
		{
			REN_CORE_ASSERT(false, "We should be saving using the provided vector serializer!");
		}

		template<class Archive>
		void load(Archive& ar)
		{
			cereal::size_type vectorSize;
			ar(cereal::make_size_tag(vectorSize));

			mEntities.resize(static_cast<std::size_t>(vectorSize));
			size_t numElements = static_cast<std::size_t>(vectorSize);
 			for (size_t i = 0; i < numElements; ++i)
 			{
 				ar(mScene->CreateEntity());
 			}
		}

	private:
		Scene* mScene;
		std::vector<Entity> mEntities;
	};
}