#pragma once

#include "Renaissance.h"
#include "Faireground/Windows/EditorWindow.h"

#include "imgui.h"

#define DRAG_CONTEXT_PATH_TEX "DRAG_CONTEXT_PATH_TEXTURE"
#define DRAG_CONTEXT_PATH_SCENE "DRAG_CONTEXT_PATH_SCENE"

namespace Renaissance
{
	class EditorLayer : public Layer
	{
	public:
		enum class EEditorUpdateMode
		{
			Editor,
			Play,
			Simulate
		};

	public:
		static WeakPtr<Scene> GetActiveScene();
		static Entity GetSelectedEntity();
		static void SetSelectedEntity(Entity entity);
		static EEditorUpdateMode GetUpdateState();

		static void NewScene();
		static void OpenScene();
		static void OpenScene(const std::filesystem::path& filepath);
		static void SaveSceneAs();

		static void StartPlayInEditor();
		static void StopPlayAndSimulate();
		static void StartSimulate();

	public:
		template <typename T, typename ... Args>
		WeakPtr<T> CreateNewWindow(Args&& ... args)
		{
			SharedPtr<T> newWindow = MakeShared<T>(std::forward<Args>(args)...);
			mWindows.emplace_back(newWindow);
			newWindow->Open();
			return newWindow;
		}

		template <typename T>
		uint32_t GetWindowCount()
		{
			uint32_t count = 0;
			for (auto iter = mWindows.begin(); iter != mWindows.end(); ++iter)
			{
				if (std::dynamic_pointer_cast<T>(*iter))
				{
					count++;
				}
			}
			return count;
		}

		template <typename T>
		WeakPtr<T> GetWindow()
		{
			for (auto iter = mWindows.begin(); iter != mWindows.end(); ++iter)
			{
				if (SharedPtr<T> window = std::dynamic_pointer_cast<T>(*iter))
				{
					return window;
				}
			}
			return nullptr;
		}

		virtual void OnAttached() override;
		virtual void OnDetached() override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnEvent(Events::Event& e) override;

	private:
		static SharedPtr<Scene> sActiveScene;
		static Archive sSceneSnapshot;
		static Entity sSelectedEntity;
		static EEditorUpdateMode sUpdateState;

		std::vector<SharedPtr<EditorWindow>> mWindows;
		
#if REN_DEBUG
		bool mShowDemoWindow = false;
#endif
	};
}