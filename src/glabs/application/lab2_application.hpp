#pragma once

#include "glabs/application/lab_task.hpp"
#include "glabs/graphics/window.hpp"
#include "glabs/rendering/dear_imgui.hpp"

namespace glabs
{
	class Lab2Application
	{
	public:
		Lab2Application();

		void Run();

		void PushTask(LabTaskKind kind);

	private:
		void InitWindow();
		void InitImGui();
		void InitTasks();

		void Update();
		void Render();

		LabTask* GetCurrentTask() const;
		void PeekNextTask();

		bool mRunning = false;

		Window mWindow;
		DearImGui mImGui;

		std::array<
			std::unique_ptr<LabTask>,
			size_t(LabTaskKind::Count_)
		> mTasks;
		LabTaskKind mCurrentTask = LabTaskKind::None;
		std::queue<LabTaskKind> mTaskQueue;
	};
}

