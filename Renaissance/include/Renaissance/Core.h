#pragma once

#ifdef REN_PLATFORM_WINDOWS
	#ifdef REN_BUILD_DLL
		#define REN_API __declspec(dllexport)
	#else 
		#define REN_API __declspec(dllimport)
	#endif
#else
	#error Renaissance engine is only compatible with Windows at this moment.
#endif