#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/RendererAPI.h"
#include "Renaissance/Graphics/RenderCommands.h"
#include "Renaissance/Math/Math.h"

namespace Renaissance::Graphics
{
	class Shader;
	class VertexArray;

	using namespace Math;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();

		static void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const Matrix4& transform = Matrix4(1.0f));
	
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}