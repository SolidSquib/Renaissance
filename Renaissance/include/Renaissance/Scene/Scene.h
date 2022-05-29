#pragma once

#include "Renaissance/Core/Core.h"

#include "entt.hpp"

namespace Renaissance
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity();
		void DestroyEntity(const Entity& entity);

		void Draw();

	private:
		entt::registry mRegistry;

		friend class Entity;
	};
}