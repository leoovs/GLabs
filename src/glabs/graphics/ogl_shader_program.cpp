#include "glabs/graphics/ogl_shader_program.hpp"

namespace glabs
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// OglUniformLocationCache
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	void OglUniformLocationCache::SetNativeShaderProgram(GLuint nativeShaderProgram)
	{
		mCachedLocations.clear();
		mNativeShaderProgram = nativeShaderProgram;
	}

	GLint OglUniformLocationCache::GetUniformLocation(const std::string& uniformName) const
	{
		if (auto it = mCachedLocations.find(uniformName); it != mCachedLocations.end())
		{
			return it->second;
		}

		GLint uniformLocation = glGetUniformLocation(mNativeShaderProgram, uniformName.data());

		if (cInvalidUniformLocation == uniformLocation)
		{
			return cInvalidUniformLocation;
		}

		return mCachedLocations.insert({ uniformName, uniformLocation }).first->second;
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// OglShaderProgram
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

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

	void OglShaderProgram::SetUniform(const std::string& name, float value)
	{
		glProgramUniform1f(
			mNativeShaderProgram,
			mUniformLocationCache.GetUniformLocation(name),
			value
		);
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

		glObjectLabel(
			GL_PROGRAM,
			mNativeShaderProgram,
			GLsizei(mParams.DebugName.length()),
			mParams.DebugName.data()
		);

		mUniformLocationCache.SetNativeShaderProgram(mNativeShaderProgram);
	}

	void OglShaderProgram::DestroyNativeShaderProgram()
	{
		glDeleteProgram(std::exchange(mNativeShaderProgram, 0));
	}
}

