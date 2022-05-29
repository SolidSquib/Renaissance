#include "RenaissancePCH.h"
#include "Renaissance/Scene/Scene.h"
#include "Renaissance/Scene/Entity.h"
#include "Renaissance/Graphics/SpriteBatch.h"

namespace Renaissance
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		mRegistry.clear();
	}

	Entity Scene::CreateEntity()
	{
		Entity entity(mRegistry.create(), this);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		mRegistry.destroy(entity);
	}

	void Scene::Draw()
	{
		using namespace Graphics;

		{			
			SpriteBatch spriteBatch;
			mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>).each([this, &spriteBatch](auto handle, TransformComponent& transform, SpriteRendererComponent& spriteRenderer) {

				spriteBatch.Draw(transform, spriteRenderer);
			});
		}
	}
}