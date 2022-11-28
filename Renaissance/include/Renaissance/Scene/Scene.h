#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/GUID.h"
#include "Renaissance/Graphics/Camera.h"

#include "entt.hpp"

#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>

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

		String MakeSnapshot();
		void RestoreSnapshot(const String& binaryString);

		template<class Archive>
		void save(Archive& ar) const
		{
			std::vector<Entity> entities;
			mRegistry.each([this, &entities](entt::entity entityId) {

				Entity entity(entityId, const_cast<Scene*>(this));
				if (!entity.IsValid())
					return;
				
				entities.push_back(entity);
			});
			ar(cereal::make_nvp("Entities", entities));
		}		

		template<class Archive>
		void load(Archive& ar)
		{
			EntityLoader loader(this);
			ar(loader);
		}

	private:
		entt::registry mRegistry;

		friend class Entity;
		friend class SceneSerializer;
	};
}