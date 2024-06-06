#include "RenaissancePCH.h"
#include "Renaissance/Core/Application.h"
#include "Renaissance/Core/InputManager.h"
#include "Renaissance/Graphics/Renderer.h"
#include "Renaissance/Graphics/SpriteBatch.h"
#include "Renaissance/Platform/Platform.h"
#include "Renaissance/Core/FileSystem.h"

namespace Renaissance
{
	Application* Application::sInstance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args)
		: mArgs(args)
	{
		REN_CORE_ASSERT(sInstance == nullptr, "An instance of application already exists!");
		sInstance = this;
		if constexpr (cereal::JSONOutputArchive::is_loading::value)
		{

		}
		WindowProperties windowProps(name);
		std::ifstream input("config/engine.json");		
		if (!input.good())
		{
			Config::SavedWindowData defaultWindow;
			defaultWindow.Width = windowProps.Width;
			defaultWindow.Height = windowProps.Height;
			defaultWindow.X = 0;
			defaultWindow.Y = 0;
			defaultWindow.Maximized = windowProps.Maximized;
			mAppSettings.Windows.push_back(defaultWindow);
		}
		else
		{
			cereal::JSONInputArchive reader(input);
			reader(mAppSettings);
		}

		windowProps.Width = mAppSettings.Windows[0].Width;
		windowProps.Height = mAppSettings.Windows[0].Height;
		windowProps.Maximized = mAppSettings.Windows[0].Maximized;
		int x = mAppSettings.Windows[0].X;
		int y = mAppSettings.Windows[0].Y;
		
		AddWindow(windowProps, x, y);

		Graphics::Renderer::Get().Init();
		Graphics::SpriteBatch::Init();
		InputManager::Get().Init();

		mImGuiLayer = CreateNewOverlay<ImGuiLayer>();
	}

	Application::~Application()
	{
		Graphics::Renderer::Get().Shutdown();
		Graphics::SpriteBatch::Shutdown();
		InputManager::Get().Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(REN_BIND_EVENT(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(REN_BIND_EVENT(Application::OnWindowResized));
		dispatcher.Dispatch<WindowMovedEvent>(REN_BIND_EVENT(Application::OnWindowMoved));

		// propogate events to other layers
		mLayerStack.OnEvent(e);
	}

	Renaissance::Window& Application::AddWindow(const WindowProperties& properties, int x /*= 0*/, int y /*= 0*/)
	{
		UniquePtr<Window>& newWindow = mWindows.emplace_back(Window::Create(properties, x, y));
		newWindow->SetEventCallback(REN_BIND_EVENT(Application::OnEvent));

		return *newWindow;
	}

	void Application::Close()
	{
		mRunning = false;

		std::ofstream output("config/engine.json");
		cereal::JSONOutputArchive writer(output);
		writer(cereal::make_nvp("EngineSettings", mAppSettings));
	}

	void Application::RegisterComponentTypes()
	{
		// Register known component types 
		TagComponent::DefineEntityMetaType();
		IdentifierComponent::DefineEntityMetaType();
		TransformComponent::DefineEntityMetaType();
		SpriteRendererComponent::DefineEntityMetaType();
		CameraComponent::DefineEntityMetaType();

		TagComponent::RegisterComponentSerializeFunctions<cereal::JSONInputArchive, cereal::JSONOutputArchive, cereal::BinaryInputArchive, cereal::BinaryOutputArchive>();
		IdentifierComponent::RegisterComponentSerializeFunctions<cereal::JSONInputArchive, cereal::JSONOutputArchive, cereal::BinaryInputArchive, cereal::BinaryOutputArchive>();
		TransformComponent::RegisterComponentSerializeFunctions<cereal::JSONInputArchive, cereal::JSONOutputArchive, cereal::BinaryInputArchive, cereal::BinaryOutputArchive>();
		SpriteRendererComponent::RegisterComponentSerializeFunctions<cereal::JSONInputArchive, cereal::JSONOutputArchive, cereal::BinaryInputArchive, cereal::BinaryOutputArchive>();
		CameraComponent::RegisterComponentSerializeFunctions<cereal::JSONInputArchive, cereal::JSONOutputArchive, cereal::BinaryInputArchive, cereal::BinaryOutputArchive>();
	}

	void Application::Run()
	{
		RegisterComponentTypes();

		Graphics::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		while (mRunning)
		{
			Graphics::RenderCommand::Clear(0);		

			float time = Platform::GetTimeSeconds();
			mDeltaTime = mLastFrameTime > 0.0f ? time - mLastFrameTime : 1.0f / 60.0f;
			mLastFrameTime = time;

			if (!mMinimized)
			{
				mLayerStack.OnUpdate(mDeltaTime);

				mImGuiLayer.lock()->BeginDraw();
				mLayerStack.RenderUI();
				mImGuiLayer.lock()->EndDraw();
			}

			for (auto& window : mWindows)
			{
				window->OnUpdate();
			}			
		}
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		uint32_t index = e.GetWindowIndex();
		mWindows.erase(mWindows.begin() + index);

		for (uint32_t i = index; index < mWindows.size(); ++i)
		{
			mWindows[i]->SetIndex(mWindows[i]->GetIndex() - 1);
		}

		if (mWindows.size() <= 0)
		{
			Close();
		}

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

		REN_CORE_ASSERT(e.GetWindowIndex() < mAppSettings.Windows.size());
		Config::SavedWindowData& windowConfig = mAppSettings.Windows[e.GetWindowIndex()];
		windowConfig.Width = e.GetWidth();
		windowConfig.Height = e.GetHeight();
		windowConfig.Maximized = e.IsMaximized();

		Graphics::Renderer::Get().OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnWindowMoved(WindowMovedEvent& e)
	{
		REN_CORE_ASSERT(e.GetWindowIndex() < mAppSettings.Windows.size());
		Config::SavedWindowData& windowConfig = mAppSettings.Windows[e.GetWindowIndex()];
		windowConfig.X = e.GetXPos();
		windowConfig.Y = e.GetYPos();

		return false;
	}
}