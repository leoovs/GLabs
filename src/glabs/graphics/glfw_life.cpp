#include "glabs/graphics/glfw_life.hpp"

#include "glabs/pch.hpp"

namespace glabs
{
	GlfwLife::GlfwLife(Params params)
		: mParams(std::move(params))
	{
		InitGlfw();
	}

	GlfwLife::~GlfwLife()
	{
		TerminateGlfw();
	}

	void GlfwLife::InitGlfw()
	{
		int result = glfwInit();
		assert(GLFW_TRUE == result);

		if (mParams.Version.first)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mParams.Version.first);
		}
		if (mParams.Version.second)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mParams.Version.second);
		}
		if (mParams.EnableDebugContext)
		{
			glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
		}
		if (mParams.CoreProfile)
		{
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}
	}

	void GlfwLife::TerminateGlfw()
	{
		glfwTerminate();
	}
}

