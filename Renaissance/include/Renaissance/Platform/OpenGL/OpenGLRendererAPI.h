#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/RendererAPI.h"

namespace Renaissance::Graphics
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() = default;

		virtual void Init() final;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) final;

		virtual void SetClearColor(const Vector4 color) final;
		virtual void Clear(uint32_t flags) final;

		virtual void DrawIndexed(const SharedPtr<VertexArray>& vertexArray) final;
	};
}