#pragma once

#include "renaissance/Core.h"

namespace Renaissance 
{
	class REN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		virtual void Init();
	};

	// Define this function in the client app:
	Application* CreateApplication();
}


