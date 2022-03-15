#include "RenaissancePCH.h"
#include "Renaissance/Core/Application.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Renaissance
{
	Application* Application::sInstance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args)
		: mArgs(args)
	{
		REN_CORE_ASSERT(sInstance == nullptr, "An instance of application already exists!");
		sInstance = this;
		mWindow = Window::Create(WindowProperties(name));
		mWindow->SetEventCallback(REN_BIND_EVENT(Application::OnEvent));

		// init renderer
		mVertexArray = Graphics::VertexArray::Create();
		
		{
			float vertices[] = {
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
			};
			uint32_t indices[] = {
				0, 1, 2
			};

			SharedPtr<Graphics::VertexBuffer> triangleVB = Graphics::VertexBuffer::Create(vertices, sizeof(vertices));
			triangleVB->SetLayout({
				{ Graphics::ShaderDataType::Float3, "a_Position" },
				{ Graphics::ShaderDataType::Float4, "a_Color" }
				});

			SharedPtr<Graphics::IndexBuffer> triangleIB = Graphics::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
			
			mVertexArray->AddVertexBuffer(triangleVB);
			mVertexArray->SetIndexBuffer(triangleIB);
		}


		std::string vertexSource = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;

			out vec4 v_Color;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
				v_Color = a_Color;
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core

			in vec4 v_Color;	

			out vec4 color;

			void main()
			{
				color = vec4(0.8, 0.0, 0.9, 1.0);
				color = v_Color;
			}
		)";

		mShader = Graphics::Shader::CreateFromSource(vertexSource.c_str(), fragmentSource.c_str());

		mImGuiLayer = CreateNewOverlay<ImGuiLayer>();
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(REN_BIND_EVENT(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(REN_BIND_EVENT(Application::OnWindowResized));

		// propogate events to other layers
		mLayerStack.OnEvent(e);
	}

	void Application::Close()
	{
		mRunning = false;
	}

	void Application::Run()
	{
		while (mRunning)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			mShader->Bind();
			mVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, mVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			mShader->Unbind();

			float time = (float)glfwGetTime();
			mDeltaTime = mLastFrameTime > 0.0f ? time - mLastFrameTime : 1.0f / 60.0f;
			mLastFrameTime = time;

			if (!mMinimized)
			{
				mLayerStack.OnUpdate(mDeltaTime);

				mImGuiLayer.lock()->BeginDraw();
				mLayerStack.RenderUI();
				mImGuiLayer.lock()->EndDraw();
			}

			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			mMinimized = true;
			return false;
		}

		mMinimized = false;
		// inform renderer

		return false;
	}
}