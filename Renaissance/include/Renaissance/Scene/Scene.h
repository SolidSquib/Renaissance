#pragma once

#include "Renaissance/Core/Archive.h"
#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/GUID.h"
#include "Renaissance/Graphics/Camera.h"

#include "entt.hpp"

namespace Renaissance
{
	class Entity;
	class SceneArchive;

	class Scene
	{
	public:
		Scene() { mRegistry.clear(); }
		virtual ~Scene();

		Entity CreateEntity(const GUID* id = nullptr);
		Entity CreateEntity(const String& name, const GUID* id = nullptr);		
		void DestroyEntity(const Entity& entity);

		void OnEditorUpdate(float deltaTime);
		void OnUpdate(float deltaTime);
		void OnRender();
		void OnRender(const Graphics::Camera& camera, const Math::Matrix4& transform);

		/// <summary>
		/// Iterator function takes a pointer to this scene, the current entity, and the currently selection context.
		/// </summary>
		typedef void(*IteratorFunction)(Scene&, Entity, Entity);
		void IterateEntities(IteratorFunction function, Entity selectionContext);

		static Archive MakeSceneSnapshot(const SharedPtr<Scene>& scene);
		static SharedPtr<Scene> RestoreSceneSnapshot(const Archive& ar);

	private:
		entt::registry mRegistry;

		friend class Entity;
		friend class SceneSerializer;
	};
}