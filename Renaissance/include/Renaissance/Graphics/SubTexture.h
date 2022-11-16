#pragma once

#include "Renaissance/Math/Math.h"
#include "Renaissance/Graphics/Texture.h"

namespace Renaissance::Graphics
{
	using namespace Renaissance::Math;

	struct SubTexture2D
	{
		SharedPtr<Texture2D> Texture;
		Vector2 MinCoord = { 0.0f, 0.0f };
		Vector2 MaxCoord = { 1.0f, 1.0f };
	};
}