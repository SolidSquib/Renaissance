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

		/// <summary>
		/// Iterator function takes a pointer to this scene, the current entity, and the currently selection context.
		/// </summary>
		typedef void(*IteratorFunction)(Scene&, Entity, Entity);
		void IterateEntities(IteratorFunction function, Entity selectionContext);

	private:
		entt::registry mRegistry;

		friend class Entity;
		friend class SceneSerializer;
	};
}