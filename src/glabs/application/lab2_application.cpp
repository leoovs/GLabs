#include "glabs/application/lab2_application.hpp"
#include "glabs/application/lab_task.hpp"
#include "glabs/application/lab_task_draw_first_shape.hpp"
#include "glabs/application/lab_task_draw_ngon.hpp"
#include "glabs/application/lab_task_draw_ngon_triangle_fan.hpp"
#include "glabs/application/lab_task_draw_second_shape_line_strip.hpp"
#include "glabs/application/lab_task_draw_second_shape_triangles.hpp"
#include "glabs/application/lab_task_draw_third_shape.hpp"
#include "glabs/graphics/ogl_debug_output.hpp"

namespace glabs
{
	Lab2Application::Lab2Application()
	{
		InitWindow();
		InitImGui();
		InitTasks();
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

	void Lab2Application::PushTask(LabTaskKind kind)
	{
		mTaskQueue.push(kind);
	}

	void Lab2Application::InitWindow()
	{
		mWindow = Window(1280, 720, "lab2");

		mWindow.SetCloseCallback([this](){ mRunning = false; });

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(OglDebugMessageCallback, nullptr);
	}

	void Lab2Application::InitImGui()
	{
		mImGui = DearImGui(DearImGui::Params{ &mWindow });

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Tahoma.ttf", 18, nullptr, io.Fonts->GetGlyphRangesCyrillic());
	}

	void Lab2Application::InitTasks()
	{
		std::unique_ptr<LabTask> tasks[]
		{
			std::make_unique<LabTask_DrawNGon>(*this),
			std::make_unique<LabTask_DrawFirstShape>(),
			std::make_unique<LabTask_DrawSecondShapeLineStrip>(),
			std::make_unique<LabTask_DrawSecondShapeTriangles>(),
			std::make_unique<LabTask_DrawNGonTriangleFan>(),
			std::make_unique<LabTask_DrawThirdShape>(),
		};

		for (std::unique_ptr<LabTask>& task : tasks)
		{
			if (task)
			{
				mTasks.at(size_t(task->GetKind())) = std::move(task);
			}
		}
	}

	void Lab2Application::Update()
	{
		mWindow.PollEvents();
		mImGui.NewFrame();

		PeekNextTask();
		if (LabTask* task = GetCurrentTask())
		{
			task->OnUpdate();

			ImGui::Begin("Главное меню");
			ImGui::Text("Версия OpenGL: %s", glGetString(GL_VERSION));

			ImGui::NewLine();
			if (task->GetPrev() != LabTaskKind::None)
			{
				if (ImGui::Button("Пред."))
				{
					PushTask(task->GetPrev());
				}
				ImGui::SameLine();
			}
			ImGui::Text("%s", GetLabTaskKindUserFriendlyString(task->GetKind()));
			if (task->GetNext() != LabTaskKind::None )
			{
				ImGui::SameLine();
				if (ImGui::Button("След."))
				{
					PushTask(task->GetNext());
				}
			}
			ImGui::Separator();

			task->OnMainMenu();
			ImGui::End();
		}

		ImGui::Render();
	}

	void Lab2Application::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, mWindow.GetWidth(), mWindow.GetHeight());

		if (LabTask* task = GetCurrentTask())
		{
			task->OnRender();
		}

		mImGui.RenderDrawData(ImGui::GetDrawData());
		mWindow.Present();
	}

	LabTask* Lab2Application::GetCurrentTask() const
	{
		return LabTaskKind::None != mCurrentTask
			? mTasks.at(size_t(mCurrentTask)).get()
			: nullptr;
	}

	void Lab2Application::PeekNextTask()
	{
		if (mTaskQueue.empty())
		{
			return;
		}

		if (LabTask* currentTask = GetCurrentTask())
		{
			currentTask->OnQuit();
		}

		mCurrentTask = mTaskQueue.front();
		mTaskQueue.pop();

		GetCurrentTask()->OnEnter();
	}
}

