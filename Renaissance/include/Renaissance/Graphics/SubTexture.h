#pragma once

#include "Renaissance/Math/Math.h"
#include "Renaissance/Graphics/Texture.h"

namespace Renaissance::Graphics
{
	using namespace Renaissance::Math;

	class SubTexture2D
	{
	public:
		~SubTexture2D() = default;

		virtual const SharedPtr<Texture2D>& GetTexture() const = 0;
		virtual const Vector2& GetMinCoord() const = 0;
		virtual const Vector2& GetMaxCoord() const = 0;

		static SharedPtr<SubTexture2D> Create(const SharedPtr<Texture2D>& texture, const Vector2& min, const Vector2& max);
	};
}