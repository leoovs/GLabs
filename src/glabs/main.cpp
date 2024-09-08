#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glabs/graphics/window.hpp"
#include "glabs/graphics/ogl_buffer.hpp"

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

	float vertices[]
	{
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.5f, 0.5f,
	};

	glabs::OglBuffer::Params bufParams;
	bufParams.DebugName = "Test buffer";
	bufParams.Target = GL_ARRAY_BUFFER;
	bufParams.ElementSize = sizeof(*vertices);
	bufParams.ElementCount = std::size(vertices);

	glabs::OglBuffer buf(bufParams);
	buf.SetData(vertices);
	buf.BindToPipeline();

	while (running)
	{
		wnd.PollEvents();
		wnd.Present();
	}
}

