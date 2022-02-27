#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Core/Window.h"
#include "Renaissance/Events/Event.h"
#include "Renaissance/Events/AppEvent.h"
#include "Renaissance/Events/WindowEvent.h"

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
		
		static Application& Get() { return *sInstance; }

	private:
		void Run();
		bool OnWindowClosed(WindowClosedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		ApplicationCommandLineArgs mArgs;
		UniquePtr<Window> mWindow;
		bool mRunning = true;
		bool mMinimized = false;
		float mLastFrameTime = 0.0f;

		static Application* sInstance;
		friend int ::main(int argc, char** argv);
	};

	// Define this function in the client app:
	Application* CreateApplication(ApplicationCommandLineArgs args);
}