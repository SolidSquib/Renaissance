#include "renaissance/Application.h"
#include "renaissance/Log.h"

namespace Renaissance
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		REN_CORE_WARN("Hello warning");
		REN_TRACE("Hello trace");
		while (true)
		{
			// run the application
		}
	}

	void Application::Init()
	{
		Log::Init();
	}
}