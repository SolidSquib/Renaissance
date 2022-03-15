#pragma once

#include "Renaissance/Graphics/Context.h"

struct GLFWwindow;

namespace Renaissance::Graphics
{
	class OpenGLContext : public Context
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() final;
		virtual void Resize(float width, float height) final;
		virtual void SwapBuffers() final;

	private:
		GLFWwindow* mWindowHandle;
	};
}