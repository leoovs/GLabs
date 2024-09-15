#pragma once

#include "glabs/graphics/window.hpp"
#include "glabs/rendering/dear_imgui.hpp"

namespace glabs
{
	class Lab2Application
	{
	public:
		Lab2Application();

		void Run();

	private:
		void InitWindow();
		void InitImGui();

		void Update();
		void Render();

		bool mRunning = false;

		Window mWindow;
		DearImGui mImGui;
	};
}

