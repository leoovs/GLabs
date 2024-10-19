#include "glabs/app/basic_app.hpp"
#include "glabs/app/imgui_app_config.hpp"
#include "glabs/app/window_app_config.hpp"
#include "glabs/graphics/ogl_debug_output.hpp"

namespace glabs
{
	void BasicApp::OnWindowClose()
	{
		Quit();
	}

	std::string_view BasicApp::GetName() const
	{
		return "Unnamed Basic App";
	}

	AppConfigurator& BasicApp::GetConfigurator() const
	{
		return *mAppConfigurator;
	}

	Window& BasicApp::GetWindow() const
	{
		return GetConfigurator().GetConfig<WindowAppConfig>().GetAppWindow();
	}

	DearImGui& BasicApp::GetImGui() const
	{
		return GetConfigurator().GetConfig<ImGuiAppConfig>().GetImGui();
	}

	void BasicApp::Quit()
	{
		OnQuit();

		mRunning = false;
	}

	bool BasicApp::IsKeyDown(int glfwKey) const
	{
		return glfwGetKey(GetWindow().GetNativeWindow(), glfwKey) & GLFW_PRESS;
	}

	bool BasicApp::IsKeyUp(int glfwKey) const
	{
		return glfwGetKey(GetWindow().GetNativeWindow(), glfwKey) & GLFW_RELEASE;
	}

	void BasicApp::Configure(AppConfigurator& configurator)
	{
		mAppConfigurator = &configurator;

		configurator.SetConfig<WindowAppConfig>(std::string(GetName()), 1280, 720);
		configurator.SetConfig<ImGuiAppConfig>();
	}

	void BasicApp::Start()
	{
		SetupWindowCallbacks();
		SetupOglDebugCallback();
		PrintOglVersion();

		mRunning = true;
		mTimer.Tick();

		OnStart();
	}

	void BasicApp::Update()
	{
		mTimer.Tick();
		GetWindow().PollEvents();
		OnUpdate(mTimer.GetDeltaTime());
	}

	bool BasicApp::IsRunning() const
	{
		return mRunning;
	}

	void BasicApp::SetupWindowCallbacks()
	{
		Window& window = GetWindow();

		window.SetCloseCallback(
			[this]()
			{
				OnWindowClose();
			}
		);

		window.SetSizeCallback(
			[this](int32_t width, int32_t height)
			{
				OnWindowResize(width, height);
			}
		);
	}

	void BasicApp::SetupOglDebugCallback()
	{
		glDebugMessageCallback(OglDebugMessageCallback, nullptr);
	}

	void BasicApp::PrintOglVersion()
	{
		std::cout << glGetString(GL_VERSION) << '\n';
	}
}

