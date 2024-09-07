#include <iostream>

#include <GLFW/glfw3.h>

int main()
{
	int result = glfwInit();
	if (result != GLFW_TRUE)
	{
		std::cerr << "failed to initialize glfw\n";
		return 1;
	}

	GLFWwindow* wnd = glfwCreateWindow(1280, 720, "Hello world", nullptr, nullptr);

	while (!glfwWindowShouldClose(wnd))
	{
		glfwPollEvents();
	}

	glfwDestroyWindow(wnd);
}

