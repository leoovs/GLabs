#pragma once

#include "glabs/app/app.hpp"
#include "glabs/graphics/window.hpp"
#include "glabs/rendering/dear_imgui.hpp"

namespace glabs
{
	class BasicApp : public App
	{
	protected:
		virtual void OnWindowClose();
		virtual void OnWindowResize(int32_t width, int32_t height) {}
		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnQuit() {}

		AppConfigurator& GetConfigurator() const;
		Window& GetWindow() const;
		DearImGui& GetImGui() const;

		void Quit();

	private:
		void Configure(AppConfigurator& configurator) override;

		void Start() override final;
		void Update() override;

		bool IsRunning() const override final;

		void SetupWindowCallbacks();
		void SetupOglDebugCallback();
		void PrintOglVersion();

		bool mRunning = false;
		AppConfigurator* mAppConfigurator = nullptr;
	};
}

