#pragma once

#include "glabs/app/app.hpp"
#include "glabs/graphics/window.hpp"
#include "glabs/rendering/dear_imgui.hpp"
#include "glabs/rendering/frame_timer.hpp"

namespace glabs
{
	class BasicApp : public App
	{
	protected:
		virtual void OnWindowClose();
		virtual void OnWindowResize(int32_t width, int32_t height) {}
		virtual void OnStart() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnQuit() {}
		virtual std::string_view GetName() const;

		AppConfigurator& GetConfigurator() const;
		Window& GetWindow() const;
		DearImGui& GetImGui() const;

		void Quit();
		bool IsKeyDown(int glfwKey) const;
		bool IsKeyUp(int glfwKey) const;

	private:
		void Configure(AppConfigurator& configurator) override;

		void Start() override final;
		void Update() override;

		bool IsRunning() const override final;

		void SetupWindowCallbacks();
		void SetupOglDebugCallback();
		void PrintOglVersion();

		bool mRunning = false;
		FrameTimer mTimer;
		AppConfigurator* mAppConfigurator = nullptr;
	};
}

