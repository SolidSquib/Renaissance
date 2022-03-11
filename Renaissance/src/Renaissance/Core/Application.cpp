#include "RenaissancePCH.h"
#include "Renaissance/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Renaissance
{
	Application* Application::sInstance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args)
		: mArgs(args)
	{
		REN_CORE_ASSERT(sInstance == nullptr, "An instance of application already exists!");
		sInstance = this;
		mWindow = Window::Create(WindowProperties(name));
		mWindow->SetEventCallback(REN_BIND_EVENT(Application::OnEvent));

		// init renderer

		mImGuiLayer = CreateNewOverlay<ImGuiLayer>();
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(REN_BIND_EVENT(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(REN_BIND_EVENT(Application::OnWindowResized));

		// propogate events to other layers
		mLayerStack.OnEvent(e);
	}

	void Application::Close()
	{
		mRunning = false;
	}

	void Application::Run()
	{
		while (mRunning)
		{
			float time = (float)glfwGetTime();
			mDeltaTime = mLastFrameTime > 0.0f ? time - mLastFrameTime : 1.0f / 60.0f;
			mLastFrameTime = time;

			if (!mMinimized)
			{
				mLayerStack.OnUpdate(mDeltaTime);

				mImGuiLayer.lock()->BeginDraw();
				mLayerStack.RenderUI();
				mImGuiLayer.lock()->EndDraw();
			}

			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			mMinimized = true;
			return false;
		}

		mMinimized = false;
		// inform renderer

		return false;
	}
}