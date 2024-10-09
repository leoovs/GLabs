#include "glabs/graphics/glfw_life.hpp"
#include "glabs/lab3/lab3app.hpp"
#include "glabs/app/app_container.hpp"

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.CoreProfile = true;
	glfwParams.Version = { 4, 6 };
	glfwParams.EnableDebugContext = true;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::AppContainer()
			.Bind<glabs::Lab3App>()
			.Configure()
			.Run();
	}
}

