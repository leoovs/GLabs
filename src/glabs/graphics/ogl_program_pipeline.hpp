#pragma once

#include "glabs/graphics/ogl_shader_program.hpp"

namespace glabs
{
	struct OglShaderProgramUsage
	{
		std::array<OglShaderProgram*, size_t(ShaderStage::Count_)> Programs{};
	};

	class OglShaderProgramSetter
	{
	public:
		OglShaderProgramSetter(
			ShaderStage stage,
			OglShaderProgramUsage& usage,
			GLuint nativeProgramPipeline
		);

		void Set(OglShaderProgram& program);
		void Unset();

	private:
		ShaderStage mSettingStage = ShaderStage::Vertex;
		OglShaderProgramUsage* mUsage = nullptr;
		GLuint mNativeProgramPipeline;
	};

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

		OglShaderProgramSetter operator[](ShaderStage stage);

	private:
		void CreateNativeProgramPipeline();
		void DestroyNativeProgramPipeline();

		Params mParams;
		GLuint mNativeProgramPipeline = 0;
		OglShaderProgramUsage mUsage;
	};
}

