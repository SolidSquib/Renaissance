#pragma once

#include "Renaissance/Development/Instrumentor.h"

#ifdef REN_PLATFORM_WINDOWS

extern Renaissance::Application* Renaissance::CreateApplication(Renaissance::ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Renaissance::Log::Init();

	REN_PROFILE_BEGIN_SESSION("startup", "RenProfile-Startup");
	Renaissance::Application* App = Renaissance::CreateApplication({argc, argv});	
	REN_PROFILE_END_SESSION();

	REN_PROFILE_BEGIN_SESSION("runtime", "RenProfile-Runtime");
	App->Run();
	REN_PROFILE_END_SESSION();

	REN_PROFILE_BEGIN_SESSION("shutdown", "RenProfile-Shutdown");
	delete App;
	REN_PROFILE_END_SESSION();

	return 0;
}

#endif