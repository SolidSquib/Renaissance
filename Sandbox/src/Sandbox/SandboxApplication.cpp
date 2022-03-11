#include <Renaissance.h>
#include "Renaissance/Core/EntryPoint.h"

using namespace Renaissance;

class SandboxApplication : public Application
{
public:
	SandboxApplication(const std::string& name, ApplicationCommandLineArgs args)
		: Application(name, args)
	{
	}

	virtual ~SandboxApplication()
	{
	}
};

Application* Renaissance::CreateApplication(Renaissance::ApplicationCommandLineArgs args)
{
	return new SandboxApplication("Renaissance Sandbox", args);
}