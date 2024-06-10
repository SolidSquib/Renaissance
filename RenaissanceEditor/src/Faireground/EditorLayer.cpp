#include "FairegroundPCH.h"
#include "Faireground/EditorLayer.h"

#include "Renaissance/Core/Serialization.h"
#include "Renaissance/Graphics/Texture.h"
#include "Renaissance/Scene/Entity.h"
#include "Renaissance/Utilities/PlatformUtilities.h"

#include "Faireground/Windows/EditorViewportWindow.h"
#include "Faireground/Windows/PropertyEditorWindow.h"
#include "Faireground/Windows/SceneHierarchyWindow.h"
#include "Faireground/Windows/ContentBrowserWindow.h"
#include "Faireground/Windows/EditorToolbarWindow.h"

#include "ImGuizmo.h"
#include "IconsFontAwesome5.h"

namespace Renaissance
{
	using namespace Graphics;	
	
	SharedPtr<Scene> EditorLayer::sActiveScene;
	String EditorLayer::sSceneSnapshot;
	Entity EditorLayer::sSelectedEntity;
	EditorLayer::EEditorUpdateMode EditorLayer::sUpdateState = EEditorUpdateMode::Editor;

	WeakPtr<Scene> EditorLayer::GetActiveScene()
	{
		return sActiveScene;
	}

	Entity EditorLayer::GetSelectedEntity()
	{
		return sSelectedEntity;
	}

	void EditorLayer::SetSelectedEntity(Entity entity)
	{
		sSelectedEntity = entity;
	}

	EditorLayer::EEditorUpdateMode EditorLayer::GetUpdateState()
	{
		return sUpdateState;
	}

	void EditorLayer::OnAttached()
	{
		Window& window = Application::Get().GetWindow();

		std::ifstream input("config/editor.json");
		if (input.fail())
		{
			CreateNewWindow<EditorViewportWindow>(0, Graphics::Camera::MakeOrthographic((float)window.GetWidth(), (float)window.GetHeight(), 2.0f, 0.1f, 500.0f));
			CreateNewWindow<SceneHierarchyWindow>();
			CreateNewWindow<PropertyEditorWindow>();
			CreateNewWindow<ContentBrowserWindow>(0);
			CreateNewWindow<EditorToolbarWindow>();
		}
		else
		{
			std::vector<String> windows;
			{
				cereal::JSONInputArchive archive(input);
				archive(windows);
			}

			uint32_t numViewports = 0;
			uint32_t numContentBrowsers = 0;
			for (auto& windowData : windows)
			{
				if (windowData == EditorToolbarWindow::GetWindowClassNameStatic() && GetWindowCount<EditorToolbarWindow>() == 0)
				{
					CreateNewWindow<EditorToolbarWindow>();
				}
				else if (windowData == SceneHierarchyWindow::GetWindowClassNameStatic() && GetWindowCount<SceneHierarchyWindow>() == 0)
				{
					CreateNewWindow<SceneHierarchyWindow>();
				}
				else if (windowData == PropertyEditorWindow::GetWindowClassNameStatic() && GetWindowCount<PropertyEditorWindow>() == 0)
				{
					CreateNewWindow<PropertyEditorWindow>();
				}
				else if (windowData == EditorViewportWindow::GetWindowClassNameStatic())
				{
					CreateNewWindow<EditorViewportWindow>(numViewports++, Graphics::Camera::MakePerspective((float)window.GetWidth(), (float)window.GetHeight(), 75.0f, 0.1f, 1000.0f));
				}
				else if (windowData == ContentBrowserWindow::GetWindowClassNameStatic())
				{
					CreateNewWindow<ContentBrowserWindow>(numContentBrowsers++);
				}
			}
		}
	}

	void EditorLayer::OnDetached()
	{
		std::ofstream output("config/editor.json");
		cereal::JSONOutputArchive archive(output);
		std::vector<String> windows;
		for (const SharedPtr<EditorWindow>& window : mWindows)
		{
			windows.push_back(window->GetWindowClassName());
		}
		archive(cereal::make_nvp("Windows", windows));

		mWindows.clear();
	}

	void EditorLayer::OnUIRender()
	{
		ImGuizmo::BeginFrame();

		static bool showEditor = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
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
			if (ImGui::MenuItem("New"))
			{
				NewScene();
			}

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				OpenScene();
			}

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
			{
				SaveSceneAs();
			}

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
				CreateNewWindow<SceneHierarchyWindow>();
			}

			if (ImGui::MenuItem("Toolbar", nullptr, false, GetWindowCount<EditorToolbarWindow>() == 0))
			{
				CreateNewWindow<EditorToolbarWindow>();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Add Viewport", nullptr))
			{
				uint32_t currentViewportCount = GetWindowCount<EditorViewportWindow>();
				WeakPtr<EditorViewportWindow> viewport = CreateNewWindow<EditorViewportWindow>(currentViewportCount);
			}			

			ImGui::Separator();

			if (ImGui::MenuItem("Add Content Browser", nullptr))
			{
				uint32_t currentContentBrowserCount = GetWindowCount<ContentBrowserWindow>();
				WeakPtr<ContentBrowserWindow> contentBrowser = CreateNewWindow<ContentBrowserWindow>(currentContentBrowserCount);
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

		if (sActiveScene)
		{
			switch (sUpdateState)
			{
			case EEditorUpdateMode::Editor:
				sActiveScene->OnEditorUpdate(deltaTime);
				break;

			case EEditorUpdateMode::Play:
			case EEditorUpdateMode::Simulate:
				sActiveScene->OnUpdate(deltaTime);
				break;
			}			
		}
	}
	
	void EditorLayer::OnEvent(Events::Event& e)
	{
		Events::EventDispatcher dispatcher(e);

	}

	void EditorLayer::NewScene()
	{
		sActiveScene = MakeShared<Scene>();
		sSelectedEntity = {};
	}

	void EditorLayer::OpenScene()
	{
		String filePath = FileDialogs::OpenFile("Ren Scene (*.rscene)\0*.rscene\0");
		OpenScene(filePath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& filepath)
	{
		if (!filepath.empty())
		{
			sActiveScene = MakeShared<Scene>();
			sSelectedEntity = {};

			std::ifstream fin(filepath);
			cereal::JSONInputArchive ar(fin);
			ar(*sActiveScene);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		String filePath = FileDialogs::SaveFile("Ren Scene (*.rscene)\0*.rscene\0");
		if (!filePath.empty())
		{
			size_t fileExtensionString = filePath.rfind(".rscene");
			if (fileExtensionString == String::npos)
			{
				filePath.append(".rscene");
			}

			std::ofstream fout(filePath);
			cereal::JSONOutputArchive ar(fout);
			ar(*sActiveScene);
		}
	}

	void EditorLayer::StartPlayInEditor()
	{
		if (!sActiveScene)
			return; 

		sSceneSnapshot = sActiveScene->MakeSnapshot();
		sUpdateState = EEditorUpdateMode::Play;
	}

	void EditorLayer::StopPlayAndSimulate()
	{
		sActiveScene->RestoreSnapshot(sSceneSnapshot);
		sSceneSnapshot = "";

		sUpdateState = EEditorUpdateMode::Editor;
	}

	void EditorLayer::StartSimulate()
	{
		
	}

}
