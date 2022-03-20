#pragma once

#include "Renaissance/Core/Core.h"

namespace Renaissance
{
	class Platform
	{
	public:
		static float GetTimeSeconds() { return sInstance->GetTimeSecondsImpl(); }
		static float GetTimeMilliseconds() { return sInstance->GetTimeMillisecondsImpl(); }

	protected:
		virtual float GetTimeSecondsImpl() const = 0;
		virtual float GetTimeMillisecondsImpl() const = 0;

	private:
		static Platform* sInstance;
	};
}