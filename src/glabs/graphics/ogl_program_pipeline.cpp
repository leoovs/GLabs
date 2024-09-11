#include "glabs/graphics/ogl_program_pipeline.hpp"

namespace glabs
{
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

