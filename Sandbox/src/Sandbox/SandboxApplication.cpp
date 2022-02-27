#include <Renaissance.h>
#include "Renaissance/Core/EntryPoint.h"

class SandboxApplication : public Renaissance::Application
{
public:
	SandboxApplication(const std::string& name, Renaissance::ApplicationCommandLineArgs args)
		: Application(name, args)
	{
	}

	virtual ~SandboxApplication()
	{
	}
};

Renaissance::Application* Renaissance::CreateApplication(Renaissance::ApplicationCommandLineArgs args)
{
	return new SandboxApplication("Renaissance Sandbox", args);
}