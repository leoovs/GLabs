#include "glabs/app/imgui_app_config.hpp"

#include "glabs/app/app_configurator.hpp"
#include "glabs/app/window_app_config.hpp"

namespace glabs
{
	void ImGuiAppConfig::Apply(AppConfigurator& configurator)
	{
		SetupImGuiContext(configurator.GetConfig<WindowAppConfig>().GetAppWindow());
	}

	void ImGuiAppConfig::Undo(AppConfigurator& configurator)
	{
		mImGui = DearImGui();
	}

	DearImGui& ImGuiAppConfig::GetImGui()
	{
		return mImGui;
	}

	void ImGuiAppConfig::SetupImGuiContext(Window& outputWindow)
	{
		DearImGui::Params imguiParams;
		imguiParams.OutputWindow = &outputWindow;

		mImGui = DearImGui(std::move(imguiParams));
	}
}

