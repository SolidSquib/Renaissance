#include "FairegroundPCH.h"
#include "Faireground/EditorLayer.h"

#include "Renaissance/Graphics/Texture.h"
#include "Renaissance/Scene/Entity.h"

namespace Renaissance
{
	void EditorLayer::OnAttached()
	{
		Window& window = Application::Get().GetWindow();

		mScene = MakeShared<Scene>();

		SharedPtr<Graphics::Camera> viewportCamera = Graphics::Camera::MakeOrthographic((float)window.GetWidth(), (float)window.GetHeight(), 1.0f, 0.1f, 500.0f);
		viewportCamera->SetLocation(Math::Vector3(0.0f, 0.0f, 0.5f));

		mViewports[0] = mWindowStack.CreateNewWindow<EditorViewportWindow>("Viewport", viewportCamera);
		mViewports[0].lock()->SetScene(mScene);

		// Set the scene
		{
			using namespace Graphics;

			SharedPtr<Texture2D> awesomeFaceTexture = Texture2D::Create("../Renaissance/assets/textures/awesomeface.png");
			SharedPtr<Texture2D> grassTexture = Texture2D::Create("../Renaissance/assets/textures/grass.png");
			SharedPtr<Texture2D> containerTexture = Texture2D::Create("../Renaissance/assets/textures/container.jpg");

			Entity awesomeFace = mScene->CreateEntity();
			awesomeFace.AddComponent<SpriteRendererComponent>(awesomeFaceTexture);

			Entity container = mScene->CreateEntity();
			container.AddComponent<SpriteRendererComponent>(containerTexture, Vector2(0.5f));
			container.SetLocation(Vector3(1.0f, -0.2f, 0.0f));

			Entity grass = mScene->CreateEntity();
			grass.AddComponent<SpriteRendererComponent>(grassTexture);
			grass.SetLocation(Vector3(-0.5f, -0.2f, 0.0f));

			class TestScriptableEntity : public ScriptableEntity
			{
			public:
				void OnCreate() {
					REN_CORE_WARN("Test object created");
				}

				void OnDestroy() {
					REN_CORE_WARN("Test object DESTROYED");
				}

				void OnUpdate(float deltaTime)
				{
					REN_CORE_WARN("Tick object");
				}
			};

			grass.AddComponent<NativeScriptComponent>().Bind<TestScriptableEntity>();
		}
	}

	void EditorLayer::OnDetached()
	{

	}

	void EditorLayer::OnUIRender()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Renaissance Editor", &mShowEditor, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);
		
		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			REN_CORE_WARN("Dear ImGui docking disabled.");
		}

		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", nullptr, false, true))
			{
				Renaissance::Application::Get().Close();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			String itemName = "";
			for (uint32_t i = 0; i < MaxViewports; ++i)
			{
				itemName = i == 0 ? "Viewport" : "Viewport " + std::to_string(i+1);
				bool viewportEnabled = !mViewports[i].expired();
				if (ImGui::MenuItem(itemName.c_str(), nullptr, viewportEnabled))
				{
					if (viewportEnabled)
					{
						mViewports[i].lock()->Close();
					}
					else
					{
						mViewports[i] = mWindowStack.CreateNewWindow<EditorViewportWindow>(itemName);
						mViewports[i].lock()->SetScene(mScene);
					}
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

		mWindowStack.OnDraw();

		ImGui::End();
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		
	}
	
	void EditorLayer::OnEvent(Events::Event& e)
	{
		
	}
}
