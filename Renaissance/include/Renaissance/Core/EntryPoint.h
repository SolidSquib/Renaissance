#pragma once

#ifdef REN_PLATFORM_WINDOWS

extern Renaissance::Application* Renaissance::CreateApplication();

int main(int argc, char** argv)
{
	Renaissance::Application* App = Renaissance::CreateApplication();
	App->Init();
	App->Run();

	delete App;
	return 0;
}

#endif