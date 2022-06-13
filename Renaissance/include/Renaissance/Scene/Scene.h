#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/Camera.h"

#include "entt.hpp"

namespace Renaissance
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const String& name);
		void DestroyEntity(const Entity& entity);

		void OnEditorUpdate(float deltaTime);
		void OnUpdate(float deltaTime);
		void OnRender(const Graphics::Camera& camera, const Math::Matrix4& transform);

		typedef void(*IteratorFunction)(const Entity&, void*);
		void IterateEntities(IteratorFunction function, void* data = nullptr);

	private:
		entt::registry mRegistry;

		friend class Entity;
	};
}