#pragma once

#include "Renaissance/Core/Core.h"
#include "Renaissance/Graphics/FrameBuffer.h"

#include "imgui.h"

namespace Renaissance
{
	class EditorWindow
	{
	public:
		virtual ~EditorWindow() = default;

		virtual void Init() { }

		virtual void OnUpdate(float deltaTime) { }
		virtual void OnDraw() = 0;
		virtual void OnEvent(Events::Event& e) { } 

		virtual void Close() = 0;

		virtual bool WantsInputFocus() const = 0;
		virtual bool WantsToClose() const = 0;
		virtual const String& GetName() const = 0;
	};

	class EditorWindowStack
	{
	public:
		EditorWindowStack() {}
		~EditorWindowStack();

		template <typename T, typename ... Args>
		WeakPtr<T> CreateNewWindow(const String& name, Args&& ... args)
		{
			SharedPtr<T> newWindow = MakeShared<T>(name, std::forward<Args>(args)...);
			mWindows.emplace_back(newWindow);
			newWindow->Init();
			return newWindow;
		}

		void OnUpdate(float deltaTime);
		void OnDraw();
		void OnEvent(Events::Event& e);

	private:
		std::vector<SharedPtr<EditorWindow>> mWindows;
	};
}