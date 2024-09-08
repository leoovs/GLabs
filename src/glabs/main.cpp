#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glabs/graphics/window.hpp"

struct GlfwLife
{
	GlfwLife()
	{
		int result = glfwInit();
		assert(GLFW_TRUE == result);
	}

	~GlfwLife()
	{
		glfwTerminate();
	}
};

int main()
{
	GlfwLife glfw;

	bool running = true;
	glabs::Window wnd(1280, 720, "Hello world");

	wnd.SetCloseCallback([&running](){ running = false; });
	wnd.SetSizeCallback([](int32_t width, int32_t height)
		{
			std::cout << width << ", " << height << '\n';
		}
	);

	std::cout << "OpenGL Version: " <<  glGetString(GL_VERSION) << '\n';

	while (running)
	{
		wnd.PollEvents();
		wnd.Present();
	}
}

