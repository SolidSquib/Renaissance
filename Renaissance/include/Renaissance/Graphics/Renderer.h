#pragma once

#include "Renaissance/Core/Core.h"

namespace Renaissance::Graphics
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL
	};

	class Renderer
	{
	public:
		static RendererAPI GetAPI() { return sAPI; }

	private:
		static RendererAPI sAPI;
	};
}