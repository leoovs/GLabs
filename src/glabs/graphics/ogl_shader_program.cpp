#include "glabs/graphics/ogl_shader_program.hpp"

namespace glabs
{
	OglShaderProgram::OglShaderProgram(Params params)
		: mParams(std::move(params))
	{
		CreateNativeShaderProgram();
	}

	OglShaderProgram::OglShaderProgram(OglShaderProgram&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeShaderProgram(std::exchange(other.mNativeShaderProgram, 0))
	{}

	OglShaderProgram& OglShaderProgram::operator=(OglShaderProgram&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		mParams = std::move(other.mParams);
		mNativeShaderProgram = std::exchange(other.mNativeShaderProgram, 0);

		return *this;
	}

	OglShaderProgram::~OglShaderProgram()
	{
		DestroyNativeShaderProgram();
	}

	const OglShaderProgram::Params& OglShaderProgram::GetParams() const
	{
		return mParams;
	}

	GLuint OglShaderProgram::GetNativeShaderProgram() const
	{
		return mNativeShaderProgram;
	}

	void OglShaderProgram::CreateNativeShaderProgram()
	{
		const char* source[1]{ mParams.Source.data() };
		mNativeShaderProgram = glCreateShaderProgramv(
			GetNativeShaderType(mParams.Stage),
			1,
			source
		);
	}

	void OglShaderProgram::DestroyNativeShaderProgram()
	{
		glDeleteProgram(std::exchange(mNativeShaderProgram, 0));
	}
}

