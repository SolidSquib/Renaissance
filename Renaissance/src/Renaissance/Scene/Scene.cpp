#include "RenaissancePCH.h"
#include "Renaissance/Scene/Scene.h"
#include "Renaissance/Scene/Entity.h"
#include "Renaissance/Graphics/SpriteBatch.h"

#include "Renaissance/Core/Application.h"

namespace Renaissance
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		mRegistry.view<NativeScriptComponent>().each([](auto handle, NativeScriptComponent& scriptComponent) {

			if (scriptComponent.mEntity)
			{
				if (scriptComponent.ScriptableOnDestroy)
					scriptComponent.ScriptableOnDestroy(&scriptComponent);

				scriptComponent.DestroyScript(&scriptComponent);
			}
		});

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

		mRegistry.view<NativeScriptComponent>().each([this](auto handle, NativeScriptComponent& scriptComponent) {

			if (!scriptComponent.mEntity)
			{
				scriptComponent.mEntity = scriptComponent.InstantiateScript();
				scriptComponent.mEntity->mEntity = { handle, this };

				if (scriptComponent.ScriptableOnCreate)
					scriptComponent.ScriptableOnCreate(&scriptComponent);
			}

			if (scriptComponent.ScriptableOnUpdate)
				scriptComponent.ScriptableOnUpdate(&scriptComponent, Application::Get().DeltaTime());
		});

		{
			SpriteBatch spriteBatch;
			mRegistry.group<TransformComponent, SpriteRendererComponent>().each([this, &spriteBatch](auto handle, TransformComponent& transform, SpriteRendererComponent& spriteRenderer) {

				spriteBatch.Draw(transform, spriteRenderer);
			});
		}
	}
}