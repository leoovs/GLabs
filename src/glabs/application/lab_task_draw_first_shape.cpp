#include "glabs/application/lab_task_draw_first_shape.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"

namespace glabs
{
	LabTask_DrawFirstShape::LabTask_DrawFirstShape()
	{
		LoadShape();
		LoadShaders();
	}

	LabTaskKind LabTask_DrawFirstShape::GetKind() const
	{
		return LabTaskKind::DrawFirstShape;
	}

	LabTaskKind LabTask_DrawFirstShape::GetNext() const
	{
		return LabTaskKind::DrawSecondShapeLineStrip;
	}

	LabTaskKind LabTask_DrawFirstShape::GetPrev() const
	{
		return LabTaskKind::DrawNGon;
	}

	void LabTask_DrawFirstShape::OnEnter()
	{
	}

	void LabTask_DrawFirstShape::OnQuit()
	{
	}

	void LabTask_DrawFirstShape::OnMainMenu()
	{
	}

	void LabTask_DrawFirstShape::OnUpdate()
	{
	}

	void LabTask_DrawFirstShape::OnRender()
	{
		mPrograms.SetProgram(mProgramLib[ShaderStage::Vertex].FromName("vert"));
		mPrograms.SetProgram(mProgramLib[ShaderStage::Fragment].FromName("frag"));
		mPrograms.BindToPipeline();
		mShapeLayout.BindToPipeline();

		glLineWidth(1.0f);
		glDisable(GL_LINE_SMOOTH);
		glDrawArrays(GL_LINE_STRIP, 0, 8);
	}

	void LabTask_DrawFirstShape::LoadShape()
	{
		float shapePositions[8 * 2]
		{
			-0.8f, 0.8f,
			-0.8f, -0.6f,
			-0.3f, -0.6f,
			-0.5f, -0.3f,
			-0.1f, 0.7f,
			0.2f, -0.1f,
			-0.1f, -0.6f,
			0.8f, -0.6f,
		};

		mShapePositions = OglBuffer(OglBuffer::Params{
			"Shape positions",
			GL_ARRAY_BUFFER,
			sizeof(float[2]),
			8
		});
		mShapePositions.SetData(shapePositions);

		OglGeometryInput::Params shapeLayoutParams;
		shapeLayoutParams.DebugName = "Shape layout";
		shapeLayoutParams.VertexBuffers[0] = &mShapePositions;
		shapeLayoutParams.IndexBuffer = nullptr;
		shapeLayoutParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};

		mShapeLayout = OglGeometryInput(std::move(shapeLayoutParams));
	}

	void LabTask_DrawFirstShape::LoadShaders()
	{
		mProgramLib[ShaderStage::Vertex].FetchFromFile("glsl/ngon_vert.glsl", "vert");
		mProgramLib[ShaderStage::Fragment].FetchFromFile("glsl/ngon_frag.glsl", "frag");

		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "Shape shaders" });
	}
}

