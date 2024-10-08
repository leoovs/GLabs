#include "glabs/app/basic_app.hpp"
#include "glabs/app/window_app_config.hpp"
#include "glabs/graphics/ogl_debug_output.hpp"

namespace glabs
{
	void BasicApp::OnWindowClose()
	{
		Quit();
	}

	AppConfigurator& BasicApp::GetConfigurator() const
	{
		return *mAppConfigurator;
	}

	Window& BasicApp::GetWindow() const
	{
		return GetConfigurator().GetConfig<WindowAppConfig>().GetAppWindow();
	}

	void BasicApp::Quit()
	{
		mRunning = false;
	}

	void BasicApp::Configure(AppConfigurator& configurator)
	{
		mAppConfigurator = &configurator;

		configurator.SetConfig<WindowAppConfig>();
	}

	void BasicApp::Start()
	{
		SetupWindowCallbacks();
		SetupOglDebugCallback();
		PrintOglVersion();
		mRunning = true;
	}

	void BasicApp::Update()
	{
		GetWindow().PollEvents();
		OnUpdate();
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

