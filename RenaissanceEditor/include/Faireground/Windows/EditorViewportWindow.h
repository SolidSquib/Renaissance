#pragma once

#include "Faireground/Windows/EditorWindow.h"

namespace Renaissance
{
	class EditorViewportWindow : public EditorWindow
	{
	public:
		EditorViewportWindow(const String& name);
		EditorViewportWindow(const String& name, const SharedPtr<Graphics::Camera>& camera);

		virtual void OnDraw() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnEvent(Events::Event& e) override;

		virtual void Close() override { mClose = true; }

		virtual bool WantsInputFocus() const override { return mUpdateCameraController; }
		virtual bool WantsToClose() const override { return mClose; }
		virtual const String& GetName() const override { return mName; }

	private:
		bool mClose = false;
		bool mUpdateCameraController = false;
		String mName;
		Vector2 mCachedViewportSize{ 1280.0f, 720.0f };

		SharedPtr<Graphics::FrameBuffer> mViewportFrameBuffer;
		SharedPtr<EditorCameraController> mViewportCameraController;
	};
}