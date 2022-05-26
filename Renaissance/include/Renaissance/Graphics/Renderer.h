#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/RendererAPI.h"
#include "Renaissance/Graphics/RenderCommands.h"
#include "Renaissance/Graphics/Shader.h"
#include "Renaissance/Math/Math.h"

namespace Renaissance::Graphics
{
	class Camera;
	class Shader;
	class VertexArray;

	using namespace Math;

	class Renderer
	{
	public:
		void Init();
		void Shutdown();

		void OnWindowResize(uint32_t width, uint32_t height);

		void BeginScene(const SharedPtr<Camera>& camera);
		void EndScene();

		void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const Matrix4& transform = Matrix4(1.0f));
	
		RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		const Matrix4& GetCachedViewProjection() const { return mCachedViewProjection; }

		static inline Renderer& Get()
		{
			if (!sInstance)
			{
				REN_CORE_INFO("Renderer instantiated.");
				sInstance = new Renderer;
			}

			return *sInstance;
		}

	private:
		Renderer() = default;

	private:
		Matrix4 mCachedViewProjection = IdentityMatrix;

		static Renderer* sInstance;
	};
}