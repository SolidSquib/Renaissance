#include "SandboxPCH.h"
#include "Renaissance/Core/EntryPoint.h"
#include "Sandbox/Sandbox2D.h"

using namespace Renaissance;
using namespace Sandbox;

class SandboxApplication : public Application
{
public:
	SandboxApplication(const std::string& name, ApplicationCommandLineArgs args)
		: Application(name, args)
	{
		mSandbox2D = CreateNewLayer<Sandbox2D>();
	}

	virtual ~SandboxApplication()
	{
	}

private:
	WeakPtr<Sandbox2D> mSandbox2D;
};

Application* Renaissance::CreateApplication(Renaissance::ApplicationCommandLineArgs args)
{
	return new SandboxApplication("Renaissance Sandbox", args);
}