#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/InputLayer.h"
#include "Renaissance/Core/LayerStack.h"
#include "Renaissance/Core/Window.h"

#include "Renaissance/Editor/CameraController.h"

#include "Renaissance/Events/AppEvent.h"
#include "Renaissance/Events/Event.h"
#include "Renaissance/Events/WindowEvent.h"

#include "Renaissance/UserInterface/ImGuiLayer.h"

#include "Renaissance/Graphics/Buffer.h"
#include "Renaissance/Graphics/Shader.h"
#include "Renaissance/Graphics/VertexArray.h"
#include "Renaissance/Graphics/Shader.h"

int main(int argc, char** argv);

namespace Renaissance 
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			REN_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name, ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void OnEvent(Event& e);

		Window& GetWindow() const { return *mWindow; }

		void Close();
		
		template <typename T, typename ... Args>
		WeakPtr<T> CreateNewLayer(Args&& ... args)
		{
			return mLayerStack.CreateNewLayer<T>(std::forward<Args>(args)...);
		}

		template <typename T, typename ... Args>
		WeakPtr<T> CreateNewOverlay(Args&& ... args)
		{
			return mLayerStack.CreateNewOverlay<T>(args...);
		}

		float DeltaTime() const { return mDeltaTime; }

		static Application& Get() { return *sInstance; }

	protected:
		WeakPtr<ImGuiLayer> mImGuiLayer;

	private:
		void Run();
		bool OnWindowClosed(WindowClosedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs mArgs;
		LayerStack mLayerStack;
		UniquePtr<Window> mWindow;
		bool mRunning = true;
		bool mMinimized = false;
		float mLastFrameTime = 0.0f;
		float mDeltaTime = 0.0f;

		static Application* sInstance;
		friend int ::main(int argc, char** argv);
	};

	// Define this function in the client app:
	Application* CreateApplication(ApplicationCommandLineArgs args);
}