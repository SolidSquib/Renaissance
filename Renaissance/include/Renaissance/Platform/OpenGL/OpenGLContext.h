#pragma once

#include "Renaissance/Graphics/Context.h"

struct GLFWwindow;

namespace Renaissance::Graphics
{
	class OpenGLContext : public Context
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void Resize(float width, float height) override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* mWindowHandle;
	};
}