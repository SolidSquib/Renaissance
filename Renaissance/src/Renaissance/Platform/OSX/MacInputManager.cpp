#include "RenaissancePCH.h"
#include "Renaissance/Core/Application.h"
#include "Renaissance/Platform/OSX/MacInputManager.h"

#include <GLFW/glfw3.h>

namespace Renaissance
{
	InputManager* InputManager::sInstance = new MacInputManager;


	bool MacInputManager::IsKeyPressedImpl(unsigned int keyCode) const
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int keyPressedResult = glfwGetKey(glfwWindow, keyCode);
		return keyPressedResult == GLFW_PRESS || keyPressedResult == GLFW_REPEAT;
	}

	bool MacInputManager::IsMouseButtonPressedImpl(unsigned int buttonCode) const
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int mouseButtonPressedResult = glfwGetMouseButton(glfwWindow, buttonCode);
		return mouseButtonPressedResult == GLFW_PRESS;
	}

	std::pair<float, float> MacInputManager::GetMousePositionImpl() const
	{
		GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double mouseX, mouseY;
		glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
		return { mouseX, mouseY };
	}

}