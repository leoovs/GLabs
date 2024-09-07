#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
	if (glfwInit() != GLFW_TRUE)
	{
		std::cerr << "failed to initialize glfw\n";
		return 1;
	}

	GLFWwindow* wnd = glfwCreateWindow(1280, 720, "Hello world", nullptr, nullptr);
	glfwMakeContextCurrent(wnd);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "failed to initialize glew\n";
		return 2;
	}

	std::cout << "OpenGL Version: " <<  glGetString(GL_VERSION) << '\n';

	while (!glfwWindowShouldClose(wnd))
	{
		glfwPollEvents();
		glfwSwapBuffers(wnd);
	}

	glfwDestroyWindow(wnd);
}

