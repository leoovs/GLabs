#include "glabs/app/app_container.hpp"
#include "glabs/lab6/lab6app.hpp"
#include "glabs/graphics/glfw_life.hpp"

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.Version = { 4, 6 };
	glfwParams.CoreProfile = true;
	glfwParams.EnableDebugContext = true;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::AppContainer()
			.Bind<glabs::Lab6App>()
			.Configure()
			.Run();
	}
}

