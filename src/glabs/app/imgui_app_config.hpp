#pragma once

#include "glabs/app/app_config.hpp"
#include "glabs/rendering/dear_imgui.hpp"

namespace glabs
{
	class ImGuiAppConfig : public AppConfig
	{
	public:
		ImGuiAppConfig() = default;

		void Apply(AppConfigurator& configurator) override;
		void Undo(AppConfigurator& configurator) override;

		DearImGui& GetImGui();

	private:
		void SetupImGuiContext(Window& outputWindow);

		DearImGui mImGui;
	};
}

