#pragma once

#include "Renaissance/Platform/Platform.h"

namespace Renaissance
{
	class WindowsPlatform : public Platform
	{
	protected:
		virtual float GetTimeSecondsImpl() const final;
		virtual float GetTimeMillisecondsImpl() const final;
	};
}