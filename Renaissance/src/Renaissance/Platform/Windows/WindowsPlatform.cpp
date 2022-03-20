#include "RenaissancePCH.h"
#include "Renaissance/Platform/Windows/WindowsPlatform.h"

#include <GLFW/glfw3.h>

namespace Renaissance
{
	Platform* Platform::sInstance = new WindowsPlatform;

	float WindowsPlatform::GetTimeSecondsImpl() const
	{
		return (float)glfwGetTime();
	}

	float WindowsPlatform::GetTimeMillisecondsImpl() const 
	{
		return (float)glfwGetTime() * 1000.0f;
	}
}