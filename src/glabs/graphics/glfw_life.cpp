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

		glfwWindowHint(GLFW_CONTEXT_DEBUG, mParams.EnableDebugContext ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mParams.OglVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mParams.OglVersionMinor);
		glfwWindowHint(
			GLFW_OPENGL_PROFILE,
			mParams.CoreProfile
				? GLFW_OPENGL_CORE_PROFILE
				: GLFW_OPENGL_COMPAT_PROFILE
		);
	}

	void GlfwLife::TerminateGlfw()
	{
		glfwTerminate();
	}
}

