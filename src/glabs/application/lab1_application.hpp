#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/window.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class Lab1Application
	{
	public:
		Lab1Application();

		void Run();

	private:
		void InitWindow();
		void SetupGeometry();
		void LoadShaders();

		void Update();
		void Render();

		void SetShader(ShaderStage stage, const std::string& name);

		bool mRunning = false;

		Window mAppWindow{ 1280, 720, "lab1" };

		OglBuffer mTrianglePositions;
		OglGeometryInput mTriangleGeometry;
		OglProgramPipeline mPrograms;
		ShaderLibrary mProgramLibrary;
	};
}

