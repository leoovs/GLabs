#include "glabs/rendering/dear_imgui.hpp"

#include "glabs/pch.hpp"

namespace glabs
{
	DearImGui::DearImGui(Params params)
		: mParams(std::move(params))
	{
		InitContext();
	}

	DearImGui::DearImGui(DearImGui&& other) noexcept
		: mParams(std::move(other.mParams))
		, mIsContextAcquired(std::exchange(other.mIsContextAcquired, false))
	{}

	DearImGui& DearImGui::operator=(DearImGui&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		ShutDownContext();
		mParams = std::move(other.mParams);
		mIsContextAcquired = std::exchange(other.mIsContextAcquired, false);

		return *this;
	}

	DearImGui::~DearImGui()
	{
		ShutDownContext();
	}

	const DearImGui::Params& DearImGui::GetParams() const
	{
		return mParams;
	}

	void DearImGui::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void DearImGui::RenderDrawData(ImDrawData* data)
	{
		ImGui_ImplOpenGL3_RenderDrawData(data);
	}

	void DearImGui::InitContext()
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(mParams.OutputWindow->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init();
	}

	void DearImGui::ShutDownContext()
	{
		if (!mIsContextAcquired)
		{
			return;
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

