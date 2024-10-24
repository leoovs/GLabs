#include "glabs/lab4/lab4app.hpp"
#include "glabs/lab4/task1.hpp"
#include "glabs/lab4/task3.hpp"

namespace glabs
{
	std::string_view Lab4App::GetName() const
	{
		return "lab4";
	}

	void Lab4App::OnStart()
	{
		SetupTasks();

		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontFromFileTTF(
			"C:/Windows/Fonts/Tahoma.ttf",
			20,
			nullptr,
			io.Fonts->GetGlyphRangesCyrillic()
		);
	}

	void Lab4App::OnUpdate(float dt)
	{
		PeekTask();

		if (Task* currentTask = GetCurrentTask())
		{
			currentTask->OnUpdate(dt);
		}

		ShowUI();
		Render();
	}

	void Lab4App::OnWindowResize(int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Lab4App::ShowUI()
	{
		GetImGui().NewFrame();

		ImGui::Begin("Главное меню");
		if (Task* currentTask = GetCurrentTask())
		{
			if (TaskName prev = currentTask->GetPrev(); prev != TaskName::None)
			{
				if (ImGui::Button("Пред."))
				{
					PushTask(prev);
				}
				ImGui::SameLine();
			}

			ImGui::Text("%s", TaskNameToStr(currentTask->GetName()).data());

			if (TaskName next = currentTask->GetNext(); next != TaskName::None)
			{
				ImGui::SameLine();
				if (ImGui::Button("След."))
				{
					PushTask(next);
				}
			}

			currentTask->OnUI();
		}
		ImGui::End();

		ImGui::Render();
	}

	void Lab4App::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Task* currentTask = GetCurrentTask())
		{
			currentTask->OnRender();
		}

		GetImGui().RenderDrawData(ImGui::GetDrawData());
		GetWindow().Present();
	}

	void Lab4App::SetupTasks()
	{
		mTasks.at(+TaskName::Task1) = std::make_unique<Task1>();
		mTasks.at(+TaskName::Task3) = std::make_unique<Task3>();

		mAwaitedTasks.push(TaskName::Task1);
	}

	void Lab4App::PushTask(TaskName task)
	{
		mAwaitedTasks.push(task);
	}

	void Lab4App::PeekTask()
	{
		if (mAwaitedTasks.size() <= 1)
		{
			return;
		}

		mAwaitedTasks.pop();
	}

	Task* Lab4App::GetCurrentTask() const
	{
		if (mAwaitedTasks.empty())
		{
			return nullptr;
		}

		TaskName current = mAwaitedTasks.front();

		if (TaskName::None == current)
		{
			return nullptr;
		}

		return mTasks.at(+current).get();
	}
}

