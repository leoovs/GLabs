#include "glabs/application/lab_task_draw_ngon_triangle_fan.hpp"
#include "glabs/application/shape_math.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"

namespace glabs
{
	LabTask_DrawNGonTriangleFan::LabTask_DrawNGonTriangleFan()
	{
		GenerateNGon();
		LoadShaders();
	}

	LabTaskKind LabTask_DrawNGonTriangleFan::GetKind() const
	{
		return LabTaskKind::DrawNGonTriangleFan;
	}

	LabTaskKind LabTask_DrawNGonTriangleFan::GetNext() const
	{
		return LabTaskKind::DrawThirdShape;
	}

	LabTaskKind LabTask_DrawNGonTriangleFan::GetPrev() const
	{
		return LabTaskKind::DrawSecondShapeTriangles;
	}

	void LabTask_DrawNGonTriangleFan::OnEnter()
	{
	}

	void LabTask_DrawNGonTriangleFan::OnQuit()
	{
	}

	void LabTask_DrawNGonTriangleFan::OnMainMenu()
	{
		if (ImGui::SliderInt("N", &mNGonSize, cNGonMinSize, cNGonMaxSize))
		{
			GenerateNGon();
		}
	}

	void LabTask_DrawNGonTriangleFan::OnUpdate()
	{
	}

	void LabTask_DrawNGonTriangleFan::OnRender()
	{
		mPrograms[ShaderStage::Fragment].Get().SetUniform("enableInterpolation", 0);

		mNGonGeometry.BindToPipeline();
		mPrograms.BindToPipeline();

		glDrawArrays(GL_TRIANGLE_FAN, 0, mNGonSize + 2);
	}

	void LabTask_DrawNGonTriangleFan::GenerateNGon()
	{
		std::vector<Vector2f> positions = CalculateNGonCorners(mNGonSize, 0.5f, { 0.0f, 0.0f });
		positions.insert(positions.end(), positions.front());
		positions.insert(positions.begin(), { 0.0f, 0.0f });

		mNGonPositions = OglBuffer(OglBuffer::Params{
			"Ngon triangle fan positions",
			GL_ARRAY_BUFFER,
			sizeof(Vector2f),
			positions.size()
		});
		mNGonPositions.SetData(positions.data());

		OglGeometryInput::Params ngonGeometryParams;
		ngonGeometryParams.DebugName = "NGon triangle fan params";
		ngonGeometryParams.VertexBuffers[0] = &mNGonPositions;
		ngonGeometryParams.IndexBuffer = nullptr;
		ngonGeometryParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};

		mNGonGeometry = OglGeometryInput(std::move(ngonGeometryParams));
	}

	void LabTask_DrawNGonTriangleFan::LoadShaders()
	{
		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "NGOn triangle fan programs" });

		mPrograms.SetProgram(mNGonShaders[ShaderStage::Vertex].FetchFromFile("glsl/random_color_vert.glsl", "ngon"));
		mPrograms.SetProgram(mNGonShaders[ShaderStage::Fragment].FetchFromFile("glsl/tone_frag.glsl", "ngon"));
	}
}

