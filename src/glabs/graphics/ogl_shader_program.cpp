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

	void OglShaderProgram::SetUniform(const std::string& name, float x1, float x2)
	{
		glProgramUniform2f(
			mNativeShaderProgram,
			mUniformLocationCache.GetUniformLocation(name),
			x1,
			x2
		);
	}

	void OglShaderProgram::SetUniform(
		const std::string& name,
		float x1,
		float x2,
		float x3
	)
	{
		glProgramUniform3f(
			mNativeShaderProgram,
			mUniformLocationCache.GetUniformLocation(name),
			x1,
			x2,
			x3
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

		DisplayBuildingStatus();
	}

	void OglShaderProgram::DestroyNativeShaderProgram()
	{
		if (0 != mNativeShaderProgram)
		{
			glDeleteProgram(std::exchange(mNativeShaderProgram, 0));
		}
	}

	void OglShaderProgram::DisplayBuildingStatus()
	{
		GLint isLinked = GL_FALSE;
		glGetProgramiv(mNativeShaderProgram, GL_LINK_STATUS, &isLinked);

		if (isLinked)
		{
			return;
		}

		GLint logLength = 0;
		glGetProgramiv(mNativeShaderProgram, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<char> logBuffer(logLength, '\0');
		glGetProgramInfoLog(mNativeShaderProgram, logBuffer.size(), nullptr, logBuffer.data());

		std::cerr << logBuffer.data() << '\n';
	}
}

