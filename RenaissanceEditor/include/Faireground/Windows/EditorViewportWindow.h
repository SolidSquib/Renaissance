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

		virtual bool WantsInputFocus() const override { return mButtonFocus != ImGuiMouseButton_COUNT; }
		virtual bool WantsToClose() const override { return !mOpen; }

		bool DrawViewportSettings();

		static String GetWindowClassNameStatic() { return "EditorViewport"; }
		virtual String GetWindowClassName() const final { return EditorViewportWindow::GetWindowClassNameStatic(); }

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(cereal::make_nvp("Index", mIndex));
		}

	private:
		bool mOpen = true;
		ImGuiMouseButton mButtonFocus = ImGuiMouseButton_COUNT;
		uint32_t mIndex;

		int mGizmoManipulateOperation = 0;
		uint32_t mGizmoManipulateSpace = 0;
		bool mGizmoEnableTranslationSnapping = false;
		bool mGizmoEnableRotationSnapping = false;
		float mGizmoTranslationSnapping = 0.5f;
		float mGizmoRotationSnapping = 45.0f;

		Vector2 mCachedViewportSize{ 1280.0f, 720.0f };
		Vector2 mViewportBounds[2];

		EditorCameraController mViewportCameraController;

		SharedPtr<Graphics::FrameBuffer> mViewportFrameBuffer;
	};
}