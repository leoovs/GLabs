#pragma once

#include "glabs/graphics/ogl_shader_program.hpp"

namespace glabs
{
	class OglProgramPipeline
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglProgramPipeline";
		};

		OglProgramPipeline(Params params);

		OglProgramPipeline(OglProgramPipeline&& other) noexcept;

		OglProgramPipeline& operator=(OglProgramPipeline&& other) noexcept;

		~OglProgramPipeline();

		GLuint GetNativeProgramPipeline() const;

		void BindToPipeline();

	private:
		void CreateNativeProgramPipeline();
		void DestroyNativeProgramPipeline();

		Params mParams;
		GLuint mNativeProgramPipeline = 0;
	};
}

