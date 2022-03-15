#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Math/Math.h"

namespace Renaissance::Graphics
{
	class VertexArray;

	using namespace Math;

	class RendererAPI
	{
	public:
		enum class API { None = 0, OpenGL = 1 };

	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const Vector4 color) = 0;
		virtual void Clear(uint32_t flags) = 0;

		virtual void DrawIndexed(const SharedPtr<VertexArray>& vertexArray) = 0;

		static API GetAPI() { return sAPI; }
		static UniquePtr<RendererAPI> Create();

	private:
		static API sAPI;
	};
}