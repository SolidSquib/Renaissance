#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Math/Math.h"

#include "Renaissance/Graphics/SubTexture.h"
#include "Renaissance/Graphics/VertexArray.h"
#include "Renaissance/Graphics/Camera.h"

namespace Renaissance
{
	class ScriptableEntity;

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Math::Matrix4& transform)
			: Transform(transform)
		{}

		void Translate(const Math::Vector3& translation);

		operator const Math::Matrix4& () const { return Transform; }

		Math::Matrix4 Transform = Math::IdentityMatrix;
	};

	struct SpriteRendererComponent 
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Math::Vector4& color, const Math::Vector2& size = Math::Vector2(1.0f))
			: Color(color), Size(size)
		{}

		SpriteRendererComponent(const SharedPtr<Graphics::SubTexture2D>& texture, const Math::Vector2& size = Math::Vector2(1.0f), const Math::Vector2& tilingFactor = Math::Vector2(1.0f), const Math::Vector4& tint = Math::Vector4(1.0f))
			: Color(tint), Size(size), TilingFactor(tilingFactor), Texture(texture)
		{}

		SpriteRendererComponent(const SharedPtr<Graphics::Texture2D>& texture, const Math::Vector2& size = Math::Vector2(1.0f), const Math::Vector2& tilingFactor = Math::Vector2(1.0f), const Math::Vector4& tint = Math::Vector4(1.0f))
			: Color(tint), Size(size), TilingFactor(tilingFactor), Texture(Graphics::SubTexture2D::Create(texture, { 0.0f, 0.0f }, { 1.0f, 1.0f }))
		{}

		SpriteRendererComponent(const SharedPtr<Graphics::Texture2D>& texture, const Math::Vector2& texMin, const Math::Vector2& texMax, const Math::Vector2& size = Math::Vector2(1.0f), const Math::Vector2& tilingFactor = Math::Vector2(1.0f), const Math::Vector4& tint = Math::Vector4(1.0f))
			: Color(tint), Size(size), TilingFactor(tilingFactor), Texture(Graphics::SubTexture2D::Create(texture, texMin, texMax))
		{}

		Math::Vector4 Color{ 1.0f };
		Math::Vector2 Size{ 1.0f };
		Math::Vector2 TilingFactor{ 1.0f };
		SharedPtr<Graphics::SubTexture2D> Texture;
	};

	struct CameraComponent
	{
		SharedPtr<Graphics::Camera> mCamera;
	};

#define DEFINE_HAS_MEMBER(member_name)                                         \
    template <typename T>                                                      \
    class has_member_##member_name                                             \
    {                                                                          \
        typedef char yes_type;                                                 \
        typedef long no_type;                                                  \
        template <typename U> static yes_type test(decltype(&U::member_name)); \
        template <typename U> static no_type  test(...);                       \
    public:                                                                    \
        static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);  \
    }

#define HAS_MEMBER(class_, member_name, ...)  has_member_##member_name<class_, __VA_ARGS__>::value

	struct NativeScriptComponent
	{
		DEFINE_HAS_MEMBER(OnCreate);
		DEFINE_HAS_MEMBER(OnDestroy);
		DEFINE_HAS_MEMBER(OnUpdate);

		template<typename T> 
		void Bind()
		{
			InstantiateScript = []() { return (ScriptableEntity*)new T(); };
			DestroyScript = [](NativeScriptComponent* self) { delete (T*)self->mEntity; self->mEntity = nullptr; };

			if constexpr (HAS_MEMBER(T, OnCreate))
				ScriptableOnCreate = [](NativeScriptComponent* self) { ((T*)self->mEntity)->OnCreate(); };
			if constexpr (HAS_MEMBER(T, OnDestroy))
				ScriptableOnDestroy = [](NativeScriptComponent* self) { ((T*)self->mEntity)->OnDestroy(); };
			if constexpr (HAS_MEMBER(T, OnUpdate))
				ScriptableOnUpdate = [](NativeScriptComponent* self, float deltaTime) { ((T*)self->mEntity)->OnUpdate(deltaTime); };
		}
		
		typedef void(*ScriptableFunctionPointer)(NativeScriptComponent*);

		ScriptableEntity*(*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);
		void(*ScriptableOnCreate)(NativeScriptComponent*);
		void(*ScriptableOnDestroy)(NativeScriptComponent*);
		void(*ScriptableOnUpdate)(NativeScriptComponent*, float);

		ScriptableEntity* mEntity = nullptr;
	};
}