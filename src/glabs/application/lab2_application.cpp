#include "glabs/application/lab2_application.hpp"

namespace glabs
{
	Lab2Application::Lab2Application()
	{
		InitWindow();
		InitImGui();
	}

	void Lab2Application::Run()
	{
		mRunning = true;

		while (mRunning)
		{
			Update();
			Render();
		}
	}

	void Lab2Application::InitWindow()
	{
		mWindow = Window(1280, 720, "lab2");

		mWindow.SetCloseCallback([this](){ mRunning = false; });
	}

	void Lab2Application::InitImGui()
	{
		mImGui = DearImGui(DearImGui::Params{ &mWindow });
	}

	void Lab2Application::Update()
	{
		mWindow.PollEvents();
		mImGui.NewFrame();
		ImGui::Render();
	}

	void Lab2Application::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		mImGui.RenderDrawData(ImGui::GetDrawData());
		mWindow.Present();
	}
}

