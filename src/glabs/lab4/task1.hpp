#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/lab4/task.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class Task1 : public Task
	{
	public:
		Task1();
		~Task1() override = default;

		void OnUI() override;
		void OnRender() override;

		TaskName GetName() const override;
		TaskName GetNext() const override;
		TaskName GetPrev() const override;

	private:
		void SetupShapes();
		void LoadShaders();

		OglBuffer mShapeVertexBuffer;
		OglGeometryInput mShapeVertexLayout;
		OglProgramPipeline mPrograms;

		ShaderLibrary mShaders;

		bool mEnableDepthTest = false;
		bool mDisplayDepthBuffer = true;
	};
}

