#include "RenaissancePCH.h"
#include "Renaissance/Core/Application.h"
#include "Renaissance/Graphics/SpriteBatch.h"
#include "Renaissance/Scene/Scene.h"
#include "Renaissance/Scene/SceneSerializer.h"
#include "Renaissance/Scene/Entity.h"

namespace Renaissance
{
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

	Entity Scene::CreateEntity(const GUID* id /*= nullptr*/)
	{
		return CreateEntity("NewEntity", id);
	}

	Entity Scene::CreateEntity(const String& tag, const GUID* id /*= nullptr*/)
	{
		Entity entity(mRegistry.create(), this);
		entity.AddComponent<TagComponent>(tag);
		entity.AddComponent<TransformComponent>();

		if (id)
		{
			entity.AddComponent<IdentifierComponent>(*id);
		}
		else
		{
			entity.AddComponent<IdentifierComponent>();
		}

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

	void Scene::OnRender()
	{
		mRegistry.view<CameraComponent, TransformComponent>().each([this](auto handle, CameraComponent& cameraComponent, TransformComponent& transformComponent) {

			if (cameraComponent.MainCamera)
			{
				OnRender(cameraComponent.Cam, transformComponent.GetTransform());
				return;
			}
		});

		//REN_CORE_WARN("No main camera found in scene, render aborted!");
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

	Archive Scene::MakeSceneSnapshot(const SharedPtr<Scene>& scene)
	{
		Archive ar;
		SceneWriter writer(scene, ar);

		entt::snapshot{ scene->mRegistry }
			.entities(writer)
			.component<IdentifierComponent, TagComponent, TransformComponent, SpriteRendererComponent, CameraComponent>(writer);

		return std::move(ar);
	}

	SharedPtr<Scene> Scene::RestoreSceneSnapshot(const Archive& ar)
	{
		SharedPtr<Scene> scene = MakeShared<Scene>();
		SceneReader reader(scene, ar);

		entt::snapshot_loader{ scene->mRegistry }
			.entities(reader)
			.component<IdentifierComponent, TagComponent, TransformComponent, SpriteRendererComponent, CameraComponent>(reader)
			.orphans();

		return scene;
	}
}