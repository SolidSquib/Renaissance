#include "SandboxPCH.h"
#include "Sandbox/ExampleLayer.h"

namespace Sandbox
{
	void ExampleLayer::OnAttached()
	{
		Renaissance::Window& window = Renaissance::Application::Get().GetWindow();

		mSceneCamera = Graphics::Camera::MakeOrthographic((float)window.GetWidth(), (float)window.GetHeight(), 1.0f, 0.1f, 100.0f);
		mSceneCamera->SetLocation(Math::Vector3(0.0f, 0.0f, 0.5f));
		mVertexArray = Graphics::VertexArray::Create();

		mCameraController = MakeShared<CameraController>(mSceneCamera);

		{
			float vertices[] = {
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
			};
			uint32_t indices[] = {
				0, 1, 2
			};

			SharedPtr<Graphics::VertexBuffer> triangleVB = Graphics::VertexBuffer::Create(vertices, sizeof(vertices));
			triangleVB->SetLayout({
				{ Graphics::ShaderDataType::Float3, "a_Position" },
				{ Graphics::ShaderDataType::Float4, "a_Color" }
				});

			SharedPtr<Graphics::IndexBuffer> triangleIB = Graphics::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

			mVertexArray->AddVertexBuffer(triangleVB);
			mVertexArray->SetIndexBuffer(triangleIB);
		}

		mShader = Graphics::Shader::CreateFromFile("../Renaissance/assets/shaders/ColorUnlit.glsl");
	}

	void ExampleLayer::OnDetached()
	{
		
	}

	void ExampleLayer::OnUpdate(float deltaTime)
	{
		mCameraController->OnUpdate(deltaTime);

		{
			Graphics::Renderer::Get().BeginScene(mSceneCamera);
			Graphics::Renderer::Get().Submit(mShader, mVertexArray);
			Graphics::Renderer::Get().EndScene();
		}
	}

}