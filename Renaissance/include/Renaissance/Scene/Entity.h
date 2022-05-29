#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Scene/Scene.h"
#include "Renaissance/Scene/Component.h"

#include "entt.hpp"

namespace Renaissance
{
	class Entity
	{
	public:
		Entity(const entt::entity& entityId, Scene* scene);
		virtual ~Entity();

		void SetLocation(const Math::Vector3& location);
		Math::Vector3 GetLocation() const;

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
			return mScene->mRegistry.all_of<T>(mHandle);
		}

		operator const entt::entity& () const { return mHandle; }

	private:
		entt::entity mHandle = entt::null;
		Scene* mScene = nullptr;
	};
}