#pragma once

#include "glabs/app/basic_app.hpp"
#include "glabs/lab4/task.hpp"

namespace glabs
{
	class Lab4App : public BasicApp
	{
	private:
		std::string_view GetName() const override;

		void OnStart() override;
		void OnUpdate(float dt) override;
		void OnWindowResize(int32_t width, int32_t height) override;

		void ShowUI();
		void Render();

		void SetupTasks();
		void PushTask(TaskName task);
		void PeekTask();
		Task* GetCurrentTask() const;

		std::queue<TaskName> mAwaitedTasks;
		std::array<std::unique_ptr<Task>, +TaskName::Count_> mTasks;
	};
}

