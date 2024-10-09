#include "glabs/lab3/lab3app.hpp"
#include "glabs/lab3/task1.hpp"

namespace glabs
{
	std::string_view Lab3App::GetName() const
	{
		return "Lab3";
	}

	void Lab3App::OnStart()
	{
		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontFromFileTTF(
			"C:/Windows/Fonts/Tahoma.ttf",
			18,
			nullptr,
			io.Fonts->GetGlyphRangesCyrillic()
		);

		io.ConfigDebugIsDebuggerPresent = true;

		SetupTasks();
		mWaitedTasks.push(TaskName::Task1);
	}

	void Lab3App::OnUpdate()
	{
		ShowMainMenu();
		Render();
		PickNextTask();
	}

	void Lab3App::OnWindowResize(int32_t width, int32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Lab3App::ShowMainMenu()
	{
		GetImGui().NewFrame();

		ImGui::Begin("Главное меню");
		if (Task* t = GetCurrentTask())
		{
			ShowTaskUI(t);
		}
		ImGui::End();

		ImGui::Render();
	}

	void Lab3App::ShowTaskUI(Task* task)
	{
			TaskName next = task->GetNext();
			TaskName prev = task->GetPrev();

			if (ImGui::Button("Пред."))
			{
				if (TaskName::None != prev)
				{
					mWaitedTasks.push(prev);
				}
			}

			ImGui::SameLine();
			ImGui::Text("%s", task->GetTitle().data());

			ImGui::SameLine();
			if (ImGui::Button("След."))
			{
				if (TaskName::None != next)
				{
					mWaitedTasks.push(next);
				}
			}

			task->OnMainMenu();
	}

	void Lab3App::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		GetImGui().RenderDrawData(ImGui::GetDrawData());

		if (Task* t = GetCurrentTask())
		{
			t->OnRender();
		}

		GetWindow().Present();
	}

	void Lab3App::SetupTasks()
	{
		mTasks = {
			std::make_unique<Task1>()
		};
	}

	Task* Lab3App::GetCurrentTask() const
	{
		if (mWaitedTasks.empty())
		{
			return nullptr;
		}

		return mTasks.at(size_t(mWaitedTasks.front())).get();
	}

	void Lab3App::PickNextTask()
	{
		return mWaitedTasks.size() <= 1
			? static_cast<void>(0)
			: mWaitedTasks.pop();
	}
}

