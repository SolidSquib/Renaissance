#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/RendererAPI.h"
#include "Renaissance/Math/Math.h"

namespace Renaissance::Graphics
{
	using namespace Math;

	class RenderCommand
	{
	public:
		inline static void Init() { sRendererAPI->Init(); }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { sRendererAPI->SetViewport(x, y, width, height); }
		inline static void SetClearColor(const Vector4 color) { sRendererAPI->SetClearColor(color); }
		inline static void Clear(uint32_t flags) { sRendererAPI->Clear(flags); }

		inline static void DrawIndexed(const SharedPtr<VertexArray>& vertexArray) { sRendererAPI->DrawIndexed(vertexArray); }
		inline static void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t count) { sRendererAPI->DrawIndexed(vertexArray, count); }

	private:
		static UniquePtr<RendererAPI> sRendererAPI;
	};
}