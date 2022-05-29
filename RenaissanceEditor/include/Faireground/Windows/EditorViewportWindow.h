#pragma once

#include "Faireground/Windows/EditorWindow.h"
#include "Faireground/EditorCameraController.h"

#include "Renaissance/Scene/Scene.h"

namespace Renaissance
{
	class EditorViewportWindow : public EditorWindow
	{
	public:
		EditorViewportWindow(const String& name);
		EditorViewportWindow(const String& name, const SharedPtr<Graphics::Camera>& camera);

		virtual void OnDraw() override;

		virtual void Close() override { mClose = true; }

		virtual bool WantsInputFocus() const override { return mFocused; }
		virtual bool WantsToClose() const override { return mClose; }
		virtual const String& GetName() const override { return mName; }

		inline void SetScene(const SharedPtr<Scene>& scene) { mScene = scene; }

	private:
		bool mClose = false;
		bool mFocused = false;

		String mName;
		Vector2 mCachedViewportSize{ 1280.0f, 720.0f };
		Vector2 mLastMousePosition{ 0.0f, 0.0f };

		SharedPtr<Graphics::FrameBuffer> mViewportFrameBuffer;
		SharedPtr<EditorCameraController> mViewportCameraController;
		SharedPtr<Scene> mScene = nullptr;
	};
}