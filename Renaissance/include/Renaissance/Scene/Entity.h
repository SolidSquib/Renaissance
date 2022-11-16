#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Scene/Scene.h"
#include "Renaissance/Scene/Component.h"

namespace Renaissance
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(const entt::entity& entityId, Scene* scene);
		virtual ~Entity();

		bool IsValid() const { return mHandle != entt::null && mScene != nullptr; }
		GUID GetGuid() const { return GetComponent<IdentifierComponent>().Guid; }

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

	private:
		entt::entity mHandle = entt::null;
		Scene* mScene = nullptr;
	};

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
}