#include "RenaissancePCH.h"
#include "Renaissance/Core/Window.h"

#if defined(REN_PLATFORM_WINDOWS)
	#include "Renaissance/Platform/Windows/WindowsWindow.h"
#endif

namespace Renaissance
{
	UniquePtr<Window> Window::Create(const WindowProperties& properties, int xPos, int yPos)
	{
	#if defined(REN_PLATFORM_WINDOWS)
		return MakeUnique<WindowsWindow>(properties, xPos, yPos);
	#else
		REN_CORE_FATAL("Unable to create window for unsupported platform. Aborting.");
		return nullptr;
	#endif
	}
}