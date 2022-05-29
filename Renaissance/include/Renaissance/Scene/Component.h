#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Math/Math.h"

#include "Renaissance/Graphics/SubTexture.h"
#include "Renaissance/Graphics/VertexArray.h"

namespace Renaissance
{
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
	};
}