#pragma once

#include "Renaissance.h"
#include "Faireground/Windows/EditorWindow.h"

namespace Renaissance
{
	class EditorToolbarWindow : public EditorWindow
	{
	public:
		virtual void OnUIRender() final;

		virtual bool WantsToClose() const final { return !mOpen; }

	private:
		bool mOpen = true;

		float mButtonSize = 50.0f;
		float mButtonPadding = 14.0f;
	};
}