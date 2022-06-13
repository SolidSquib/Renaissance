#include "FairegroundPCH.h"
#include "Faireground/EditorLayer.h"

#include "Renaissance/Graphics/Texture.h"
#include "Renaissance/Scene/Entity.h"

#include "Faireground/Windows/EditorViewportWindow.h"
#include "Faireground/Windows/PropertyEditorWindow.h"
#include "Faireground/Windows/SceneHierarchyWindow.h"

namespace Renaissance
{
	MulticastDelegate<void(const Entity&)> EditorLayer::OnSelectionChanged;

	void EditorLayer::OnAttached()
	{
		mActiveScene = MakeShared<Scene>();
		Window& window = Application::Get().GetWindow();

		WeakPtr<EditorViewportWindow> viewport = CreateNewWindow<EditorViewportWindow>(0, Graphics::Camera::MakeOrthographic((float)window.GetWidth(), (float)window.GetHeight(), 2.0f, 0.1f, 500.0f));
		viewport.lock()->SetScene(mActiveScene);

		CreateNewWindow<SceneHierarchyWindow>(mActiveScene);

		// Set the scene
		{
			using namespace Graphics;

			SharedPtr<Texture2D> awesomeFaceTexture = Texture2D::Create("../Renaissance/assets/textures/awesomeface.png");
			SharedPtr<Texture2D> grassTexture = Texture2D::Create("../Renaissance/assets/textures/grass.png");
			SharedPtr<Texture2D> containerTexture = Texture2D::Create("../Renaissance/assets/textures/container.jpg");

			Entity awesomeFace = mActiveScene->CreateEntity("Awesome Face");
			awesomeFace.AddComponent<SpriteRendererComponent>(awesomeFaceTexture);

			Entity container = mActiveScene->CreateEntity("Container");
			container.AddComponent<SpriteRendererComponent>(containerTexture, Vector2(0.5f));
			container.SetLocation(Vector3(1.0f, -0.2f, -2.0f));

			Entity grass = mActiveScene->CreateEntity("Grass");
			grass.AddComponent<SpriteRendererComponent>(grassTexture);
			grass.SetLocation(Vector3(-0.5f, -0.2f, -2.0f));

			class TestScriptableEntity : public ScriptableEntity
			{
			public:
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
		mWindows.clear();
	}

	void EditorLayer::OnUIRender()
	{
		static bool showEditor = true;
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
		ImGui::Begin("Renaissance Editor", &showEditor, window_flags);
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
			if (ImGui::MenuItem("Property Editor", nullptr, false, GetWindowCount<PropertyEditorWindow>() == 0))
			{
				CreateNewWindow<PropertyEditorWindow>();
			}

			if (ImGui::MenuItem("Scene Hierarchy", nullptr, false, GetWindowCount<SceneHierarchyWindow>() == 0))
			{
				CreateNewWindow<SceneHierarchyWindow>(mActiveScene);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Add Viewport", nullptr))
			{
				uint32_t currentViewportCount = GetWindowCount<EditorViewportWindow>();
				WeakPtr<EditorViewportWindow> viewport = CreateNewWindow<EditorViewportWindow>(currentViewportCount);
				viewport.lock()->SetScene(mActiveScene);
			}			

			#if REN_DEBUG
			ImGui::Separator();
			if (ImGui::MenuItem("ImGui Demo Window", nullptr, false, !mShowDemoWindow))
			{
				mShowDemoWindow = true;
			}
			#endif


			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

		ImGui::End();

		#if REN_DEBUG
		if (mShowDemoWindow)
			ImGui::ShowDemoWindow(&mShowDemoWindow);
		#endif

		for (auto it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			(*it)->OnUIRender();
		}
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		bool wantsInput = false;

		for (auto it = mWindows.begin(); it != mWindows.end();)
		{
			(*it)->OnUpdate(deltaTime);

			wantsInput |= (*it)->WantsInputFocus();

			if ((*it)->WantsToClose())
			{
				(*it)->Close();
				it = mWindows.erase(it);
			}
			else
			{
				++it;
			}
		}

		Application::Get().GetImGuiLayer().lock()->SetBlockEvents(!wantsInput);
	}
	
	void EditorLayer::OnEvent(Events::Event& e)
	{
		
	}
}
