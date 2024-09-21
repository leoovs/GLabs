#include "glabs/application/lab_task_draw_second_shape_triangles.hpp"

namespace glabs
{
	LabTask_DrawSecondShapeTriangles::LabTask_DrawSecondShapeTriangles()
	{
		LoadShape();
		LoadShapeIndicies();
		LoadShaders();
	}

	LabTaskKind LabTask_DrawSecondShapeTriangles::GetKind() const
	{
		return LabTaskKind::DrawSecondShapeTriangles;
	}

	LabTaskKind LabTask_DrawSecondShapeTriangles::GetNext() const
	{
		return LabTaskKind::DrawNGonTriangleFan;
	}

	LabTaskKind LabTask_DrawSecondShapeTriangles::GetPrev() const
	{
		return LabTaskKind::DrawSecondShapeLineStrip;
	}

	void LabTask_DrawSecondShapeTriangles::OnEnter()
	{
	}

	void LabTask_DrawSecondShapeTriangles::OnQuit()
	{
	}

	void LabTask_DrawSecondShapeTriangles::OnMainMenu()
	{
		bool drawStrategyChanged = ImGui::ListBox(
			"Вариант построения",
			&mDrawStrategyPeek,
			[](void* userdata, int option) -> const char*
			{
				return GetDrawStrategyString(static_cast<DrawStrategy>(option));
			},
			nullptr,
			size_t(DrawStrategy::Count_)
		);

		if (drawStrategyChanged)
		{
			mDrawStrategy = static_cast<DrawStrategy>(mDrawStrategyPeek);
			LoadShapeIndicies();
		}

		ImGui::Checkbox("Интерполяция цветов", &mIsColorInterpolationEnabled);
	}

	void LabTask_DrawSecondShapeTriangles::OnUpdate()
	{
	}

	void LabTask_DrawSecondShapeTriangles::OnRender()
	{
		mShapeGeometry.BindToPipeline();

		mPrograms[ShaderStage::Fragment].Get().SetUniform(
			"enableInterpolation",
			static_cast<int>(mIsColorInterpolationEnabled)
		);
		mPrograms[ShaderStage::Vertex].Get().SetUniform(
			"buildOption",
			static_cast<int>(mDrawStrategy)
		);
		mPrograms.BindToPipeline();

		switch (mDrawStrategy)
		{
		case DrawStrategy::Triangles:
			glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, nullptr);
			break;
		case DrawStrategy::TriangleStrip:
			glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_INT, nullptr);
			break;
		case DrawStrategy::TriangleFan:
			glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, nullptr);
			break;
		default:
			break;
		}
	}

	void LabTask_DrawSecondShapeTriangles::LoadShape()
	{
		float positions[2 * 8]
		{
			0.0f, -0.9f,
			-0.5f, -0.6f,
			-0.6f, 0.5f,
			-0.4f, 0.9f,
			0.0f, 0.6f,
			0.6f, 0.9f,
			0.6f, 0.2f,
			-0.2f, 0.2f,
		};

		mShapePositions = OglBuffer(OglBuffer::Params{
			"Shape positions",
			GL_ARRAY_BUFFER,
			sizeof(float[2]),
			8
		});
		mShapePositions.SetData(positions);
	}

	void LabTask_DrawSecondShapeTriangles::LoadShapeIndicies()
	{
		uint32_t indicesTriangles[18]
		{
			0, 1, 7,
			7, 1, 2,
			7, 2, 3,
			7, 3, 4,
			7, 4, 6,
			6, 4, 5,
		};

		uint32_t indicesTriangleStrip[9]
		{
			0, 1, 7,
			2,
			3,
			7,
			4,
			6,
			5,
		};

		uint32_t indicesTriangleFan[8]
		{
			7,
			0, 1, 2, 3, 4, 5, 6,
		};
		uint32_t* indices = nullptr;

		OglBuffer::Params indicesParams;
		indicesParams.DebugName = "Shape indicies";
		indicesParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		indicesParams.ElementSize = sizeof(uint32_t);

		switch (mDrawStrategy)
		{
		case DrawStrategy::Triangles:
			indicesParams.ElementCount = std::size(indicesTriangles);
			indices = indicesTriangles;
			break;
		case DrawStrategy::TriangleStrip:
			indicesParams.ElementCount = std::size(indicesTriangleStrip);
			indices = indicesTriangleStrip;
			indices = indicesTriangleStrip;
			break;
		case DrawStrategy::TriangleFan:
			indicesParams.ElementCount = std::size(indicesTriangleFan);
			indices = indicesTriangleFan;
			break;
		default:
			break;
		}

		mShapeIndicies = OglBuffer(std::move(indicesParams));
		mShapeIndicies.SetData(indices);

		OglGeometryInput::Params shapeGeomParams;
		shapeGeomParams.DebugName = "Shape geometry";
		shapeGeomParams.IndexBuffer = &mShapeIndicies;
		shapeGeomParams.VertexBuffers[0] = &mShapePositions;
		shapeGeomParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};

		mShapeGeometry = OglGeometryInput(std::move(shapeGeomParams));
	}

	void LabTask_DrawSecondShapeTriangles::LoadShaders()
	{
		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "Shape shaders" });

		mPrograms.SetProgram(mShapeShaders[ShaderStage::Vertex].FetchFromFile("glsl/random_color_vert.glsl", "shape"));
		mPrograms.SetProgram(mShapeShaders[ShaderStage::Fragment].FetchFromFile("glsl/tone_frag.glsl", "shape"));
	}
}

