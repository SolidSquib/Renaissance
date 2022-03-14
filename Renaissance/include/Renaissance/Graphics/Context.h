#pragma once

#include "Renaissance/Core/Core.h"

namespace Renaissance::Graphics
{
	class Context
	{
	public: 
		virtual ~Context() { }

		virtual void Init() = 0;
		virtual void Resize(float width, float height) = 0;
		virtual void SwapBuffers() = 0;
	};
}