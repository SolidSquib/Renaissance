#pragma once

#include "Renaissance/Graphics/SubTexture.h"

namespace Renaissance::Graphics
{
	class OpenGLSubTexture2D : public SubTexture2D
	{
	public:
		OpenGLSubTexture2D(const SharedPtr<Texture2D>& texture, const Vector2& min, const Vector2& max)
			: mTexture(texture), mMin(min), mMax(max)
		{}

		virtual const SharedPtr<Texture2D>& GetTexture() const final { return mTexture; }
		virtual const Vector2& GetMinCoord() const final { return mMin; }
		virtual const Vector2& GetMaxCoord() const final { return mMax; }

	private:
		SharedPtr<Texture2D> mTexture;
		Vector2 mMin, mMax;
	};
}