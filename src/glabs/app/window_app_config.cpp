#include "glabs/app/window_app_config.hpp"

namespace glabs
{
	WindowAppConfig::WindowAppConfig(std::string title, int32_t width, int32_t height)
		: mAppWindowTitle(std::move(title))
		, mAppWindowWidth(width)
		, mAppWindowHeight(height)
	{}

	void WindowAppConfig::Apply(AppConfigurator& configurator)
	{
		mAppWindow = Window(mAppWindowWidth, mAppWindowHeight, mAppWindowTitle);
	}

	void WindowAppConfig::Undo(AppConfigurator& configurator)
	{
		mAppWindow = Window();
	}

	void WindowAppConfig::PollEvents()
	{
		mAppWindow.PollEvents();
	}

	Window& WindowAppConfig::GetAppWindow()
	{
		return mAppWindow;
	}
}

