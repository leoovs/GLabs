#pragma once

#include "glabs/app/basic_app.hpp"
#include "glabs/lab3/task.hpp"

namespace glabs
{
	class Lab3App : public BasicApp
	{
	private:
		std::string_view GetName() const override;

		void OnStart() override;
		void OnUpdate() override;
		void OnWindowResize(int32_t width, int32_t height) override;

		void ShowMainMenu();
		void ShowTaskUI(Task* task);
		void Render();

		void SetupTasks();
		Task* GetCurrentTask() const;
		void PickNextTask();

		std::array<std::unique_ptr<Task>, size_t(TaskName::Count_)> mTasks;
		std::queue<TaskName> mWaitedTasks;
	};
}

