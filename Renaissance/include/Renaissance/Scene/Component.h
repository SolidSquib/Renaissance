#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/GUID.h"
#include "Renaissance/Math/Math.h"

#include "Renaissance/Graphics/SubTexture.h"
#include "Renaissance/Graphics/VertexArray.h"
#include "Renaissance/Graphics/Camera.h"

#include "Renaissance/Scene/Entity.h"

#include <entt.hpp>

inline namespace literals {

	/**
	 * @brief User defined literal for hashed strings.
	 * @param str The literal without its suffix.
	 * @return A properly initialized hashed string.
	 */
	[[nodiscard]] constexpr entt::hashed_string operator"" _hs(const char* str, std::size_t) noexcept {
		return entt::hashed_string{ str };
	}

	/**
	 * @brief User defined literal for hashed wstrings.
	 * @param str The literal without its suffix.
	 * @return A properly initialized hashed wstring.
	 */
	[[nodiscard]] constexpr entt::hashed_wstring operator"" _hws(const wchar_t* str, std::size_t) noexcept {
		return entt::hashed_wstring{ str };
	}

} // namespace literals

namespace Renaissance
{
	template<typename Type>
	Type& GetComponent(entt::registry* registry, entt::entity entity)
	{
		REN_CORE_ASSERT(registry);
		REN_CORE_ASSERT(registry->all_of<Type>(entity));
		return registry->get<Type>(entity);
	}

	template<typename Type>
	Type& AddComponent(entt::registry* registry, entt::entity entity)
	{
		REN_CORE_ASSERT(registry);
		REN_CORE_ASSERT(!registry->all_of<Type>(entity));
		return registry->emplace<Type>(entity);
	}

	template<typename Type>
	void ExtendEnttMetaType()
	{
		static_assert(false, "Must provide an implementation per component type. Use the DEFINE_COMPONENT_META macro!");
	}

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

#define DEFINE_HAS_TEMPLATE_MEMBER(member_name)                                \
    template <typename T, typename TemplatedType>                              \
    class has_member_##member_name                                             \
    {                                                                          \
        typedef char yes_type;                                                 \
        typedef long no_type;                                                  \
        template <typename U> static yes_type test(decltype(&U::member_name<TemplatedType>)); \
        template <typename U> static no_type  test(...);                       \
    public:                                                                    \
        static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);  \
    }

#define HAS_MEMBER(class_, member_name, ...)  has_member_##member_name<class_, __VA_ARGS__>::value

#define DEFINE_COMPONENT_META(TypeOfComponent)														\
	DEFINE_HAS_TEMPLATE_MEMBER(serialize);															\
	DEFINE_HAS_TEMPLATE_MEMBER(save);																\
	DEFINE_HAS_TEMPLATE_MEMBER(load);																\
	DEFINE_HAS_TEMPLATE_MEMBER(save_minimal);														\
	DEFINE_HAS_TEMPLATE_MEMBER(load_minimal);														\
	template<class Archive>																			\
	void SerializeComponent(Archive* ar)															\
	{																								\
		REN_CORE_ASSERT(ar);																		\
		if constexpr (HAS_MEMBER(TypeOfComponent, serialize, Archive))								\
			CEREAL_SERIALIZE_FUNCTION_NAME<Archive>(*ar);											\
		else																						\
		{																							\
			if constexpr (Archive::is_loading::value)												\
			{																						\
				if constexpr (HAS_MEMBER(TypeOfComponent, load, Archive))							\
					CEREAL_LOAD_FUNCTION_NAME<Archive>(*ar); 										\
				else if constexpr (HAS_MEMBER(TypeOfComponent, load_minimal, Archive))				\
					CEREAL_LOAD_MINIMAL_FUNCTION_NAME<Archive>(*ar);								\
			}																						\
			else																					\
			{																						\
				if constexpr (HAS_MEMBER(TypeOfComponent, save, Archive))							\
					CEREAL_SAVE_FUNCTION_NAME<Archive>(*ar);										\
				else if constexpr (HAS_MEMBER(TypeOfComponent, save_minimal, Archive))				\
					CEREAL_SAVE_MINIMAL_FUNCTION_NAME<Archive>(*ar);								\
			}																						\
		}																							\
	}																								\
	template<class Archive>																			\
	static void RegisterComponentSerializeFunction()												\
	{																								\
		auto factory = entt::meta<TypeOfComponent>();												\
		factory.func<&TypeOfComponent::SerializeComponent<Archive>>("serialize"_hs);				\
	}																								\
	template<typename... Archives>																	\
	static void RegisterComponentSerializeFunctions()												\
	{																								\
		auto factory = entt::meta<TypeOfComponent>();												\
		(factory.func<&TypeOfComponent::SerializeComponent<Archives>>("serialize"_hs), ...);		\
	}																								\
	static void DefineEntityMetaType()																\
	{																								\
		auto factory = entt::meta<TypeOfComponent>();												\
		factory.prop("name"_hs, std::move(String(REN_STRINGIFY_MACRO(TypeOfComponent))));			\
		factory.prop("id"_hs, REN_STRINGIFY_MACRO(TypeOfComponent)_hs);								\
		factory.func<&GetComponent<TypeOfComponent>, entt::as_ref_t>("get"_hs);						\
		factory.func<&AddComponent<TypeOfComponent>, entt::as_ref_t>("add"_hs);						\
	}

	class ScriptableEntity;

	struct TagComponent
	{
		TagComponent() : Tag("None") { }
		TagComponent(const String& name) : Tag(name) { }

		String Tag;

		String GetName() const { return "TagComponent"; }
																										
		DEFINE_COMPONENT_META(TagComponent);

	private:
		friend class cereal::access;

		template<class Ar>
		void serialize(Ar& ar)
		{
			ar(cereal::make_nvp("Tag", Tag));
		}

// 		template <class Archive>
// 		String save_minimal(Archive const&) const
// 		{
// 			REN_CORE_TRACE("SAVE TAG COMPONENT");
// 			return Tag;
// 		}
// 
// 		template <class Archive>
// 		void load_minimal(Archive const&, String const& value)
// 		{
// 			Tag = value;
// 		}
	};
	
	struct IdentifierComponent
	{
		IdentifierComponent() : Guid() { }
		IdentifierComponent(const GUID& id) : Guid(id) { }
		IdentifierComponent(uint64_t id) : Guid(id) { }

		GUID Guid;
		
		DEFINE_COMPONENT_META(IdentifierComponent);

	private:
		friend class cereal::access;

		template<class Ar>
		void serialize(Ar& ar)
		{
			ar(CEREAL_NVP(Guid));
		}

// 		template <class Archive>
// 		uint64_t save_minimal(Archive const&) const
// 		{
// 			REN_CORE_TRACE("ID");
// 			return (uint64_t)Guid;
// 		}
// 
// 		template <class Archive>
// 		void load_minimal(Archive const&, uint64_t const& value)
// 		{
// 			Guid = value;
// 		}		
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Math::Vector3& location, const Math::Vector3& rotation, const Math::Vector3& scale = Math::Vector3(1.0f))
			: Location(location), Rotation(rotation), Scale(scale)
		{}

		void Translate(const Math::Vector3& translation);
		Math::Matrix4 GetTransform() const
		{
			return glm::translate(Math::IdentityMatrix, Location) * glm::toMat4(Math::Quat(glm::radians(Rotation))) * glm::scale(Math::IdentityMatrix, Scale);
		}

		operator Math::Matrix4() const { return GetTransform(); }

		Math::Vector3 Location{ 0.0f };
		Math::Vector3 Rotation{ 0.0f }; // Pitch, Yaw, Roll
		Math::Vector3 Scale{ 1.0f };

		DEFINE_COMPONENT_META(TransformComponent);

	private:
		friend class cereal::access;

		template <class Ar>
		void serialize(Ar& ar)
		{
			ar(CEREAL_NVP(Location), CEREAL_NVP(Rotation), CEREAL_NVP(Scale));
		}
	};	

	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Math::Vector4& color, const Math::Vector2& size = Math::Vector2(1.0f))
			: Color(color), Size(size)
		{}

		SpriteRendererComponent(const Graphics::SubTexture2D& texture, const Math::Vector2& size = Math::Vector2(1.0f), const Math::Vector2& tilingFactor = Math::Vector2(1.0f), const Math::Vector4& tint = Math::Vector4(1.0f))
			: Color(tint), Size(size), TilingFactor(tilingFactor), Texture(texture)
		{}

		SpriteRendererComponent(const SharedPtr<Graphics::Texture2D>& texture, const Math::Vector2& size = Math::Vector2(1.0f), const Math::Vector2& tilingFactor = Math::Vector2(1.0f), const Math::Vector4& tint = Math::Vector4(1.0f))
			: Color(tint), Size(size), TilingFactor(tilingFactor), Texture({ texture, { 0.0f, 0.0f }, { 1.0f, 1.0f } })
		{}

		SpriteRendererComponent(const SharedPtr<Graphics::Texture2D>& texture, const Math::Vector2& texMin, const Math::Vector2& texMax, const Math::Vector2& size = Math::Vector2(1.0f), const Math::Vector2& tilingFactor = Math::Vector2(1.0f), const Math::Vector4& tint = Math::Vector4(1.0f))
			: Color(tint), Size(size), TilingFactor(tilingFactor), Texture({ texture, texMin, texMax })
		{}

		Math::Vector4 Color{ 1.0f };
		Math::Vector2 Size{ 1.0f };
		Math::Vector2 TilingFactor{ 1.0f };
		Graphics::SubTexture2D Texture;

		DEFINE_COMPONENT_META(SpriteRendererComponent);

	private:
		friend class cereal::access;

		template<class Ar>
		void save(Ar& ar) const
		{
			ar(CEREAL_NVP(Color), CEREAL_NVP(Size), CEREAL_NVP(TilingFactor), cereal::make_nvp("MinCoord", Texture.MinCoord), cereal::make_nvp("MaxCoord", Texture.MaxCoord));

			String texturePath = Texture.Texture ? Texture.Texture->GetPath().string() : "";
			ar(cereal::make_nvp("Texture", texturePath));
		}

		template<class Ar>
		void load(Ar& ar)
		{
			ar(Color, Size, TilingFactor, Texture.MinCoord, Texture.MaxCoord);

			String filepath;
			ar(cereal::make_nvp("Texture", filepath));

			if (!filepath.empty())
			{
				Texture.Texture = Graphics::TextureLibrary::GetGlobal().FindOrLoad<Graphics::Texture2D>(filepath);
			}
		}
	};

	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const Graphics::Camera& camera) : Cam(camera)
		{}

		Math::Matrix4 GetProjectionMatrix() const { return Cam.GetProjectionMatrix(); }

		float GetViewportWidth() const { return Cam.GetViewportWidth(); }
		float GetViewportHeight() const { return Cam.GetViewportHeight(); }
		float GetFieldOfView() const { return Cam.GetFieldOfView(); }
		float GetOrthoScale() const { return Cam.GetOrthoScale(); }
		float GetNearClipping() const { return Cam.GetNearClipping(); }
		float GetFarClipping() const { return Cam.GetFarClipping(); }
		bool IsOrthographic() const { return Cam.IsOrthographic(); }

		void SetViewportSize(float width, float height) { Cam.SetViewportSize(width, height); }
		void SetFieldOfView(float fovDegrees) { Cam.SetFieldOfView(fovDegrees); }
		void SetOrthoScale(float orthoScale) { Cam.SetOrthoScale(orthoScale); }
		void SetNearClipping(float nearClip) { Cam.SetNearClipping(nearClip); }
		void SetFarClipping(float farClip) { Cam.SetFarClipping(farClip); }
		void SetOrthographic() { Cam.SetOrthographic(); }
		void SetPerspective() { Cam.SetPerspective(); }

		Graphics::Camera& GetCameraMutable() { return Cam; }
		const Graphics::Camera& GetCamera() const { return Cam; }
		void SetCamera(const Graphics::Camera& camera) { Cam = camera; }

		operator const Graphics::Camera& () const { return GetCamera(); }

		Graphics::Camera Cam;
		bool MainCamera = true;

		DEFINE_COMPONENT_META(CameraComponent);

	private:
		friend class cereal::access;

		template<class Ar>
		void serialize(Ar& ar)
		{

		}
	};

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

		ScriptableEntity* (*InstantiateScript)() = nullptr;
		void(*DestroyScript)(NativeScriptComponent*) = nullptr;
		void(*ScriptableOnCreate)(NativeScriptComponent*) = nullptr;
		void(*ScriptableOnDestroy)(NativeScriptComponent*) = nullptr;
		void(*ScriptableOnUpdate)(NativeScriptComponent*, float) = nullptr;

		ScriptableEntity* mEntity = nullptr;
	};
}