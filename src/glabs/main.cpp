#include "glabs/graphics/glfw_life.hpp"
#include "glabs/app/app_container.hpp"
#include "glabs/app/basic_app.hpp"

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.CoreProfile = true;
	glfwParams.Version = { 4, 6 };
	glfwParams.EnableDebugContext = true;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::AppContainer appRunner;

		appRunner.Bind<glabs::BasicApp>()
			.Configure()
			.Run();
	}
}

