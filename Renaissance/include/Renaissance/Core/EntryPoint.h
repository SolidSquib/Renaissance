#pragma once

#ifdef REN_PLATFORM_WINDOWS

extern Renaissance::Application* Renaissance::CreateApplication(Renaissance::ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	Renaissance::Log::Init();

	Renaissance::Application* App = Renaissance::CreateApplication({argc, argv});	
	App->Run();
	delete App;

	return 0;
}

#endif