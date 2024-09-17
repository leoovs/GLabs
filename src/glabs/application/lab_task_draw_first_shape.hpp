#pragma once

#include "glabs/application/lab_task.hpp"
#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/rendering/shader_library.hpp"
#include <cwctype>

namespace glabs
{
	class LabTask_DrawFirstShape : public LabTask
	{
	public:
		LabTask_DrawFirstShape();
		~LabTask_DrawFirstShape() override = default;

		LabTaskKind GetKind() const override;
		LabTaskKind GetNext() const override;
		LabTaskKind GetPrev() const override;

		void OnEnter() override;
		void OnQuit() override;

		void OnMainMenu() override;
		void OnUpdate() override;
		void OnRender() override;

	private:
		void LoadShape();
		void LoadShaders();

		OglBuffer mShapePositions;
		OglGeometryInput mShapeLayout;
		OglProgramPipeline mPrograms;
		ShaderLibrary mProgramLib;
	};
}

