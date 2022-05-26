#include "SandboxPCH.h"
#include "Sandbox/Sandbox2D.h"

#include "Renaissance/Graphics/Texture.h"

namespace Sandbox
{
	void Sandbox2D::OnAttached()
	{
		Renaissance::Window& window = Renaissance::Application::Get().GetWindow();

		mSceneCamera = Graphics::Camera::MakeOrthographic((float)window.GetWidth(), (float)window.GetHeight(), 1.0f, 0.1f, 500.0f);
		mSceneCamera->SetLocation(Math::Vector3(0.0f, 0.0f, 0.5f));
		mCameraController = MakeShared<CameraController>(mSceneCamera);

		mAwesomeFace = Graphics::Texture2D::Create("../Renaissance/assets/textures/awesomeface.png");
		mGrass = Graphics::Texture2D::Create("../Renaissance/assets/textures/grass.png");
		mContainer = Graphics::Texture2D::Create("../Renaissance/assets/textures/container.jpg");
	}

	void Sandbox2D::OnDetached()
	{

	}

	void Sandbox2D::OnUIRender()
	{

	}

	void Sandbox2D::OnUpdate(float deltaTime)
	{
		mCameraController->OnUpdate(deltaTime);

		{
			Graphics::Renderer::Get().BeginScene(mSceneCamera);
			
			{
				Graphics::SpriteBatch spriteBatch;
				spriteBatch.Draw(Vector2(0.0f, 0.0f), Vector2(1.0f), mAwesomeFace, Vector2(0.0f), Vector2(1.0f), Vector2(1.0f));
				spriteBatch.Draw(Vector2(-0.2f, 0.4f), Vector2(0.5f), mGrass, Vector2(0.0f), Vector2(1.0f), Vector2(1.0f));
				spriteBatch.Draw(Vector2(-0.5f, -0.2f), Vector2(0.2f), mAwesomeFace, Vector2(0.0f), Vector2(1.0f), Vector2(1.0f));
				spriteBatch.Draw(Vector2(0.6f, 0.1f), Vector2(0.2f), mContainer, Vector2(0.0f), Vector2(1.0f), Vector2(1.0f));
				spriteBatch.Draw(Vector2(0.6f, 0.1f), Vector2(0.2f), Vector4(1.0f, 0.2f, 0.4f, 1.0f));
			}

			Graphics::Renderer::Get().EndScene();
		}
	}

	void Sandbox2D::OnEvent(Events::Event& e)
	{

	}
}
