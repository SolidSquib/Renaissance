#pragma once

#include "Renaissance/Core/Layer.h"

namespace Renaissance
{
	class Events::Event;

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttached() override;
		virtual void OnDetached() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnUIRender() override;
		virtual void OnEvent(Events::Event& e) override;

		void BeginDraw();
		void EndDraw();

		void SetBlockEvents(bool block) { mBlockEvents = block; }

	private:
		bool mBlockEvents = true;
	};
}
