#include "glabs/application/lab2_application.hpp"
#include "glabs/graphics/glfw_life.hpp"

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.EnableDebugContext = false;
	glfwParams.CoreProfile = false;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::Lab2Application app;
		app.PushTask(glabs::LabTaskKind::DrawNGon);
		app.Run();
	};
}

