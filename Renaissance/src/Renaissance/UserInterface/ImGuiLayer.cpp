#include "RenaissancePCH.h"
#include "Renaissance/UserInterface/ImGuiLayer.h"
#include "Renaissance/Core/Application.h"

#include "imgui.h"

#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

namespace Renaissance
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttached()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		// TODO: read this config from an external file.
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetached()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(float deltaTime)
	{
		
	}

	void ImGuiLayer::OnUIRender()
	{
		ImGui::ShowDemoWindow();
	}

	void ImGuiLayer::OnEvent(Events::Event& e)
	{
		if (mBlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.mHandled |= e.HasCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
			e.mHandled |= e.HasCategory(EventCategoryMouse) & io.WantCaptureMouse;
		}
	}

	void ImGuiLayer::BeginDraw()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();		
	}

	void ImGuiLayer::EndDraw()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* currentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(currentContext);
		}
	}
}

