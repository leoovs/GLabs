#include "glabs/application/lab_task_draw_second_shape_line_strip.hpp"

namespace glabs
{
	LabTask_DrawSecondShapeLineStrip::LabTask_DrawSecondShapeLineStrip()
	{
		LoadShape();
		LoadShaders();
	}

	LabTaskKind LabTask_DrawSecondShapeLineStrip::GetKind() const
	{
		return LabTaskKind::DrawSecondShapeLineStrip;
	}

	LabTaskKind LabTask_DrawSecondShapeLineStrip::GetNext() const
	{
		return LabTaskKind::DrawSecondShapeTriangles;
	}

	LabTaskKind LabTask_DrawSecondShapeLineStrip::GetPrev() const
	{
		return LabTaskKind::DrawFirstShape;
	}

	void LabTask_DrawSecondShapeLineStrip::OnEnter()
	{
	}

	void LabTask_DrawSecondShapeLineStrip::OnQuit()
	{
	}

	void LabTask_DrawSecondShapeLineStrip::OnMainMenu()
	{
	}

	void LabTask_DrawSecondShapeLineStrip::OnUpdate()
	{
	}

	void LabTask_DrawSecondShapeLineStrip::OnRender()
	{
		mShapeShaders.SetProgram(mProgramLib[ShaderStage::Vertex].FromName("line_strip"));
		mShapeShaders.SetProgram(mProgramLib[ShaderStage::Fragment].FromName("line_strip"));
		mShapeShaders.BindToPipeline();

		mShapeGeometry.BindToPipeline();

		glDrawArrays(GL_LINE_STRIP, 0, 9);
	}

	void LabTask_DrawSecondShapeLineStrip::LoadShape()
	{
		float positions[2 * (8 + 1)]
		{
			0.0f, -0.9f,
			-0.5f, -0.6f,
			-0.6f, 0.5f,
			-0.4f, 0.9f,
			0.0f, 0.6f,
			0.6f, 0.9f,
			0.6f, 0.2f,
			-0.2f, 0.2f,
			0.0f, -0.9f,
		};

		mShapePositions = OglBuffer(OglBuffer::Params{
			"shape line junctions",
			GL_ARRAY_BUFFER,
			sizeof(float[2]),
			9,
		});
		mShapePositions.SetData(positions);

		OglGeometryInput::Params shapeGeometryParams;
		shapeGeometryParams.DebugName = "shape geometry for lines";
		shapeGeometryParams.VertexBuffers[0] = &mShapePositions;
		shapeGeometryParams.IndexBuffer = nullptr;
		shapeGeometryParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};

		mShapeGeometry = OglGeometryInput(std::move(shapeGeometryParams));
	}

	void LabTask_DrawSecondShapeLineStrip::LoadShaders()
	{
		mShapeShaders = OglProgramPipeline(OglProgramPipeline::Params{ "Shape shaders line strip" });

		mProgramLib[ShaderStage::Vertex].FetchFromFile("glsl/default_vert.glsl", "line_strip");
		mProgramLib[ShaderStage::Fragment].FetchFromFile("glsl/single_color_frag.glsl", "line_strip");
	}
}

