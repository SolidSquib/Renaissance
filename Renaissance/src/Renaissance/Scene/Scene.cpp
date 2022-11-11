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
		static uint32_t unnamedEntityCount = 0;
		return CreateEntity("Entity_" + std::to_string(unnamedEntityCount++));
	}

	Entity Scene::CreateEntity(const String& name)
	{
		Entity entity(mRegistry.create(), this);
		entity.AddComponent<IdentifierComponent>(name);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		mRegistry.destroy((entt::entity)entity);
	}

	void Scene::OnEditorUpdate(float deltaTime)
	{

	}

	void Scene::OnUpdate(float deltaTime)
	{
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
	}

	void Scene::OnRender(const Graphics::Camera& camera, const Math::Matrix4& transform)
	{
		{
			using namespace Graphics;

			Renderer::Get().BeginScene(camera, transform);

			{
				SpriteBatch spriteBatch;
				mRegistry.group<TransformComponent, SpriteRendererComponent>().each([this, &spriteBatch](entt::entity handle, TransformComponent& transform, SpriteRendererComponent& spriteRenderer) {

					spriteBatch.Draw(transform, spriteRenderer, (uint32_t)handle);
					});
			}

			Renderer::Get().EndScene();
		}
	}

	void Scene::IterateEntities(IteratorFunction function, Entity selectionContext)
	{
		if (function)
		{
			mRegistry.each([&](entt::entity handle) {

				function(*this, Entity(handle, this), selectionContext);
			});
		}
	}
}