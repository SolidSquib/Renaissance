#pragma once

#include "Renaissance.h"
#include <string>

namespace Renaissance
{
	class FileDialogs
	{
	public:
		// Return empty string on dialog cancel
		static String OpenFile(const char* filter);

		// Return empty string on dialog cancel
		static String SaveFile(const char* filter);
	};
}