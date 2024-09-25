#include "glabs/application/lab2_application.hpp"
#include "glabs/graphics/glfw_life.hpp"

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.EnableDebugContext = true;
	glfwParams.CoreProfile = false;
	// glfwParams.OglVersionMajor = 4;
	// glfwParams.OglVersionMinor = 6;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::Lab2Application app;
		app.PushTask(glabs::LabTaskKind::DrawNGon);
		app.Run();
	};
}

