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

	void EditorWindowStack::OnUpdate(float deltaTime)
	{
		bool wantsInput = false;

		for (auto it = mWindows.begin(); it != mWindows.end();)
		{
			(*it)->OnUpdate(deltaTime);

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

	void EditorWindowStack::OnDraw()
	{
		for (auto it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			(*it)->OnDraw();
		}
	}

	void EditorWindowStack::OnEvent(Events::Event& e)
	{
		for (auto it = mWindows.begin(); it != mWindows.end(); ++it)
		{
			(*it)->OnEvent(e);
		}
	}
}
