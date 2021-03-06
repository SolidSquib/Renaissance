#include "RenaissancePCH.h"
#include "Renaissance/Graphics/Camera.h"
#include "Renaissance/Graphics/Shader.h"
#include "Renaissance/Graphics/Renderer.h"
#include "Renaissance/Graphics/RenderCommands.h"
#include "Renaissance/Graphics/VertexArray.h"

namespace Renaissance::Graphics
{
	Renderer* Renderer::sInstance = nullptr;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{
		
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Graphics::Camera& camera, const Matrix4& transform)
	{
		mCachedViewProjection = camera.GetProjectionMatrix() * glm::inverse(transform);
	}

	void Renderer::EndScene()
	{
		
	}

	void Renderer::Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const Matrix4& transform /*= Matrix4(1.0f)*/)
	{
		shader->Bind();
		shader->SetMatrix4("u_ViewProjection", mCachedViewProjection);
		shader->SetMatrix4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}