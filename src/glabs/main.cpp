#include "glabs/app/app_container.hpp"
#include "glabs/graphics/glfw_life.hpp"
#include "glabs/lab4/lab4app.hpp"

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.Version = { 4, 6 };
	glfwParams.CoreProfile = true;
	glfwParams.EnableDebugContext = true;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::AppContainer()
			.Bind<glabs::Lab4App>()
			.Configure()
			.Run();
	}
}

