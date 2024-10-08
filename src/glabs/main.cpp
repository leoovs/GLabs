#include "glabs/graphics/glfw_life.hpp"

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.EnableDebugContext = true;
	glfwParams.CoreProfile = false;
	// glfwParams.OglVersionMajor = 4;
	// glfwParams.OglVersionMinor = 6;

	glabs::GlfwLife glfw(std::move(glfwParams));
}

