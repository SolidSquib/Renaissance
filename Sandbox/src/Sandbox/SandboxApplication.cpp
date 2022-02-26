#include <Renaissance.h>
#include "Renaissance/Core/EntryPoint.h"

class SandboxApplication : public Renaissance::Application
{
public:
	SandboxApplication()
	{
	}

	virtual ~SandboxApplication()
	{

	}
};

Renaissance::Application* Renaissance::CreateApplication()
{
	return new SandboxApplication();
}