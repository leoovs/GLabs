#include "glabs/application/lab_task_draw_third_shape.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	LabTask_DrawThirdShape::LabTask_DrawThirdShape()
	{
		LoadShape();
		LoadShaders();
	}

	LabTaskKind LabTask_DrawThirdShape::GetKind() const
	{
		return LabTaskKind::DrawThirdShape;
	}

	LabTaskKind LabTask_DrawThirdShape::GetNext() const
	{
		return LabTaskKind::DrawNGon;
	}

	LabTaskKind LabTask_DrawThirdShape::GetPrev() const
	{
		return LabTaskKind::DrawNGonTriangleFan;
	}

	void LabTask_DrawThirdShape::OnEnter()
	{
	}

	void LabTask_DrawThirdShape::OnQuit()
	{
	}

	void LabTask_DrawThirdShape::OnMainMenu()
	{
		ImGui::Checkbox("Интерполяция цветов", &mIsColorInterpolationEnabled);
		ImGui::Checkbox("Отобразить заднюю поверхность", &mIsRenderedCounterClockWise);

		bool strategyChanged = ImGui::ListBox(
			"Отображение",
			&mDisplayStrategyPeek,
			[](void* userdata, int option)
			{
				return GetDisplayStrategyString(static_cast<DisplayStrategy>(option));
			},
			nullptr,
			int(DisplayStrategy::Count_)
		);

		if (strategyChanged)
		{
			mDisplayStrategy = static_cast<DisplayStrategy>(mDisplayStrategyPeek);
		}
	}

	void LabTask_DrawThirdShape::OnUpdate()
	{
	}

	void LabTask_DrawThirdShape::OnRender()
	{
		switch (mDisplayStrategy)
		{
		case DisplayStrategy::A:
			glPointSize(5.0f);
			glPolygonMode(GL_FRONT, GL_POINT);
			glPolygonMode(GL_BACK, GL_FILL);
			glPointSize(1.0f);
			break;
		case DisplayStrategy::B:
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_LINE);
			break;
		case DisplayStrategy::C:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		default:
			break;
		}

		glFrontFace(mIsRenderedCounterClockWise ? GL_CCW : GL_CW);

		mPrograms[ShaderStage::Fragment].Get().SetUniform("enableInterpolation", static_cast<int>(mIsColorInterpolationEnabled));

		mPrograms.BindToPipeline();
		mShapeGeometry.BindToPipeline();

		glDrawElements(GL_TRIANGLES, 21, GL_UNSIGNED_INT, nullptr);

		glFrontFace(GL_CW);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void LabTask_DrawThirdShape::LoadShape()
	{
		float positions[2 * 11]
		{
			-0.1f, -0.9f,
			-0.8f, -0.9f,
			-0.5f, -0.4f,
			-0.8f, 0.6f,
			-0.1f, 0.1f,
			-0.3f, 0.4f,

			0.2f, 0.6f,
			0.2f, 0.1f,
			0.2f, -0.9f,
			0.6f, 0.9f,
			0.7f, -0.9f,
		};

		mShapePositions = OglBuffer(OglBuffer::Params{
			"Third shape pos",
			GL_ARRAY_BUFFER,
			sizeof(float[2]),
			11
		});
		mShapePositions.SetData(positions);

		uint32_t indicies[21]
		{
			0, 1, 2,
			1, 3, 4,
			3, 5, 4,
			4, 5, 6,
			4, 6, 7,
			8, 6, 9,
			8, 9, 10,
		};

		mShapeIndices = OglBuffer(OglBuffer::Params{
			"Third shape pos",
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(uint32_t),
			21
		});
		mShapeIndices.SetData(indicies);

		OglGeometryInput::Params shapeGeomParams;
		shapeGeomParams.DebugName = "Third shape geometry";
		shapeGeomParams.VertexBuffers[0] = &mShapePositions;
		shapeGeomParams.IndexBuffer = &mShapeIndices;
		shapeGeomParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};
		mShapeGeometry = OglGeometryInput(std::move(shapeGeomParams));
	}

	void LabTask_DrawThirdShape::LoadShaders()
	{
		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "Third shape shaders" });

		mPrograms.SetProgram(mShapeShaders[ShaderStage::Vertex].FetchFromFile("glsl/random_color_vert.glsl", "shape"));
		mPrograms.SetProgram(mShapeShaders[ShaderStage::Fragment].FetchFromFile("glsl/tone_frag.glsl", "shape"));
	}
}

