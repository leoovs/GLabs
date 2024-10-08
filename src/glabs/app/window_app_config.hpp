#pragma once

#include "glabs/app/app_config.hpp"
#include "glabs/graphics/window.hpp"

namespace glabs
{
	class WindowAppConfig : public AppConfig
	{
	public:
		WindowAppConfig() = default;
		WindowAppConfig(std::string title, int32_t width, int32_t height);

		void Apply(AppConfigurator& configurator) override;
		void Undo(AppConfigurator& configurator) override;

		Window& GetAppWindow();
		void PollEvents();

	private:
		std::string mAppWindowTitle = "Unnamed App Window";
		int32_t mAppWindowWidth = 1280;
		int32_t mAppWindowHeight = 720;
		Window mAppWindow;
	};
}

