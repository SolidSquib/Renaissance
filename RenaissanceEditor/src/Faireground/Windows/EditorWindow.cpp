#include "FairegroundPCH.h"
#include "Faireground/Windows/EditorWindow.h"

namespace Renaissance
{
	//////////////////////////////////////////////////////////////////////////
	//// EditorWindowStack ///////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	EditorWindowStack::~EditorWindowStack()
	{
		mWindows.clear();
	}

	void EditorWindowStack::OnDraw()
	{
		bool wantsInput = false;

		for (auto it = mWindows.begin(); it != mWindows.end();)
		{
			(*it)->OnDraw();

			wantsInput |= (*it)->WantsInputFocus();

			if ((*it)->WantsToClose())
			{
				it = mWindows.erase(it);
			}
			else
			{
				++it;
			}
		}

		Application::Get().GetImGuiLayer().lock()->SetBlockEvents(!wantsInput);
	}
}
