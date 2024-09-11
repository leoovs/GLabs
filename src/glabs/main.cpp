#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_debug_output.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/window.hpp"

struct GlfwLife
{
	GlfwLife(bool debugContext = false)
	{
		int result = glfwInit();
		assert(GLFW_TRUE == result);

		if (debugContext)
		{
			glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
		}
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

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glabs::OglDebugMessageCallback, nullptr);

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

	glabs::OglBuffer buf(std::move(bufParams));
	buf.SetData(vertices);

	glabs::OglGeometryInput::Params ogiParams;
	ogiParams.DebugName = "Test geomtry input";
	ogiParams.VertexBuffers[1] = &buf;
	ogiParams.IndexBuffer = nullptr;
	ogiParams.Vertices = {
		glabs::VertexParams{ 1, glabs::VertexFormat::Float2 }
	};

	glabs::OglGeometryInput ogi(std::move(ogiParams));
	ogi.BindToPipeline();

	glabs::OglShaderProgram::Params vertexShaderParams;
	vertexShaderParams.DebugName = "My Vertex Shader";
	vertexShaderParams.Stage = glabs::ShaderStage::Vertex;
	vertexShaderParams.Source =
R"(
#version 460 core

uniform float inX;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	gl_Position = vec4(inX, 1.0f, 0.0f, 0.0f);
}
)";

	glabs::OglShaderProgram vertexShader(std::move(vertexShaderParams));

	glabs::OglProgramPipeline::Params progPipelineParams;
	progPipelineParams.DebugName = "My program pipeline";

	glabs::OglProgramPipeline progPipeline(std::move(progPipelineParams));
	progPipeline.SetProgram(vertexShader);
	progPipeline.BindToPipeline();

	progPipeline[glabs::ShaderStage::Vertex].Get().SetUniform("inX", 56.0f);

	while (running)
	{
		wnd.PollEvents();
		wnd.Present();
	}
}

