#pragma once

#include "Renaissance/Core/Core.h"

namespace Renaissance::Graphics
{
	class VertexArray
	{
	public:
		~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		static VertexArray* Create();
	};
}