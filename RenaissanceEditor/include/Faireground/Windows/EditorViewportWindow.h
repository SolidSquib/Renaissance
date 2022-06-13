#pragma once

#include "Faireground/Windows/EditorWindow.h"
#include "Faireground/EditorCameraController.h"

#include "Renaissance/Scene/Scene.h"

namespace Renaissance
{
	class EditorViewportWindow : public EditorWindow
	{
	public:
		EditorViewportWindow(uint32_t viewportIndex);
		EditorViewportWindow(uint32_t viewportIndex, const Graphics::Camera& camera);

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnUIRender() override;

		virtual void Close() override { mOpen = false; }

		virtual bool WantsInputFocus() const override { return mFocused; }
		virtual bool WantsToClose() const override { return !mOpen; }

		inline void SetScene(const SharedPtr<Scene>& scene) { mScene = scene; }

	private:
		bool mOpen = true;
		bool mFocused = false;
		String mViewportName = "Viewport";

		Vector2 mCachedViewportSize{ 1280.0f, 720.0f };
		Vector2 mLastMousePosition{ 0.0f, 0.0f };

		EditorCameraController mViewportCameraController;

		SharedPtr<Graphics::FrameBuffer> mViewportFrameBuffer;
		SharedPtr<Scene> mScene = nullptr;
	};
}