#include "SandboxPCH.h"
#include "Renaissance/Core/EntryPoint.h"
#include "Sandbox/ExampleLayer.h"

using namespace Renaissance;
using namespace Sandbox;

class SandboxApplication : public Application
{
public:
	SandboxApplication(const std::string& name, ApplicationCommandLineArgs args)
		: Application(name, args)
	{
		mExampleLayer = CreateNewLayer<ExampleLayer>();
	}

	virtual ~SandboxApplication()
	{
	}

private:
	WeakPtr<ExampleLayer> mExampleLayer;
};

Application* Renaissance::CreateApplication(Renaissance::ApplicationCommandLineArgs args)
{
	return new SandboxApplication("Renaissance Sandbox", args);
}