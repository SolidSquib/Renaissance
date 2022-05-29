#pragma once

#include "Renaissance.h"
#include "Faireground/Windows/EditorViewportWindow.h"

#include "imgui.h"

namespace Renaissance
{
	class EditorLayer : public Layer
	{
		static const uint32_t MaxViewports = 4;

	public:
		virtual void OnAttached() override;
		virtual void OnDetached() override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnEvent(Events::Event& e) override;

	private:
		std::array<WeakPtr<EditorViewportWindow>, MaxViewports> mViewports;
		EditorWindowStack mWindowStack;

		SharedPtr<Scene> mScene;

		bool mShowEditor = true;
	};
}