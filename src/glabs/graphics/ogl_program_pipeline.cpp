#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/ogl_shader_program.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// OglShaderProgramSetter
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	OglShaderProgramSetter::OglShaderProgramSetter(
		ShaderStage stage,
		OglShaderProgramUsage& usage,
		GLuint nativeProgramPipeline
	)
		: mSettingStage(stage)
		, mUsage(&usage)
		, mNativeProgramPipeline(nativeProgramPipeline)
	{}

	void OglShaderProgramSetter::Set(OglShaderProgram& program)
	{
		ShaderStage stage = program.GetParams().Stage;
		assert(stage == mSettingStage);

		GLbitfield nativeStageBit = GetNativeShaderStageBit(stage);
		glUseProgramStages(
			mNativeProgramPipeline,
			nativeStageBit,
			program.GetNativeShaderProgram()
		);

		mUsage->Programs[size_t(stage)] = &program;
	}

	void OglShaderProgramSetter::Unset()
	{
		GLbitfield nativeStageBit = GetNativeShaderStageBit(mSettingStage);
		glUseProgramStages(mNativeProgramPipeline, nativeStageBit, 0);
		mUsage->Programs[size_t(mSettingStage)] = nullptr;
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// OglProgramPipeline
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	OglProgramPipeline::OglProgramPipeline(Params params)
		: mParams(std::move(params))
	{
		CreateNativeProgramPipeline();
	}

	OglProgramPipeline::OglProgramPipeline(OglProgramPipeline&& other) noexcept
		: mNativeProgramPipeline(std::exchange(other.mNativeProgramPipeline, 0))
	{}

	OglProgramPipeline& OglProgramPipeline::operator=(OglProgramPipeline&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeProgramPipeline();
		mNativeProgramPipeline = std::exchange(other.mNativeProgramPipeline, 0);

		return *this;
	}

	OglProgramPipeline::~OglProgramPipeline()
	{
		DestroyNativeProgramPipeline();
	}

	GLuint OglProgramPipeline::GetNativeProgramPipeline() const
	{
		return mNativeProgramPipeline;
	}

	void OglProgramPipeline::BindToPipeline()
	{
		glBindProgramPipeline(mNativeProgramPipeline);
	}

	OglShaderProgramSetter OglProgramPipeline::operator[](ShaderStage stage)
	{
		return OglShaderProgramSetter(stage, mUsage, mNativeProgramPipeline);
	}

	void OglProgramPipeline::CreateNativeProgramPipeline()
	{
		glCreateProgramPipelines(1, &mNativeProgramPipeline);
		glObjectLabel(
			GL_PROGRAM_PIPELINE,
			mNativeProgramPipeline,
			GLsizei(mParams.DebugName.length()),
			mParams.DebugName.data()
		);
	}

	void OglProgramPipeline::DestroyNativeProgramPipeline()
	{
		glDeleteProgramPipelines(1, &mNativeProgramPipeline);
		mNativeProgramPipeline = 0;
	}
}
