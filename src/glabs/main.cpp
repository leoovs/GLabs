#include "glabs/graphics/glfw_life.hpp"
#include "glabs/app/app_container.hpp"
#include "glabs/app/basic_app.hpp"

class TestApp : public glabs::BasicApp
{
private:
	void OnStart() override
	{
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	void OnUpdate() override
	{
		GetImGui().NewFrame();

		DoUI();
		Render();
	}

	void DoUI()
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin("Test app");
		ImGui::End();

		ImGui::Render();
	}

	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		GetImGui().RenderDrawData(ImGui::GetDrawData());
		GetWindow().Present();
	}
};

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.CoreProfile = true;
	glfwParams.Version = { 4, 6 };
	glfwParams.EnableDebugContext = true;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::AppContainer appRunner;

		appRunner.Bind<TestApp>()
			.Configure()
			.Run();
	}
}

