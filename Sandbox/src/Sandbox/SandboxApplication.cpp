#include <Renaissance.h>

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