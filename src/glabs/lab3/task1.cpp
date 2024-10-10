#include "glabs/lab3/task1.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	std::string_view Task1::SubtaskToStr(SubtaskName name)
	{
		switch (name)
		{
		case SubtaskName::A:
			return "а)";
		case SubtaskName::B:
			return "б)";
		case SubtaskName::C:
			return "в)";
		case SubtaskName::D:
			return "г)";
		default:
			return "???";
		}
	}

	Task1::Task1()
	{
		LoadNGonPositions();
		LoadShaders();
		UpdateNGonIndices();
	}

	TaskName Task1::GetName() const
	{
		return TaskName::Task1;
	}

	TaskName Task1::GetNext() const
	{
		return TaskName::Task2;
	}

	TaskName Task1::GetPrev() const
	{
		return TaskName::Task3;
	}

	std::string_view Task1::GetTitle() const
	{
		return "Задание 1";
	}

	void Task1::OnMainMenu()
	{
		bool subtaskChanged = ImGui::Combo(
			"Подзадание",
			reinterpret_cast<int*>(&mCurrentSubtask),
			[](void* userdata, int index) { return SubtaskToStr(SubtaskName(index)).data(); },
			nullptr,
			int(SubtaskName::Count_)
		);

		if (subtaskChanged)
		{
			UpdateNGonIndices();
		}
	}

	void Task1::OnRender()
	{
		mNGonGeometry.BindToPipeline();
		mPrograms.BindToPipeline();

		glPolygonMode(GL_FRONT_AND_BACK, mNGonPolygonMode);
		glDrawElements(mNGonDrawMode, mNGonDrawCount, GL_UNSIGNED_INT, nullptr);
	}

	Task1::NGon Task1::GenerateNGon()
	{
		NGon ngon;
		constexpr float cAngle = 2.0f * glm::pi<float>() / ngon.size();

		for (size_t iSide = 0; iSide < ngon.size(); iSide++)
		{
			ngon.at(iSide) = glm::vec2
			{
				cNGonRadius *	std::cosf(iSide * cAngle),
				cNGonRadius *	std::sinf(iSide * cAngle),
			};
		}

		return ngon;
	}

	void Task1::LoadNGonPositions()
	{
		NGon positions = GenerateNGon();

		OglBuffer::Params ngonPositionsParams;
		ngonPositionsParams.DebugName = "NGon positions";
		ngonPositionsParams.Target = GL_ARRAY_BUFFER;
		ngonPositionsParams.ElementCount = cNGonSize;
		ngonPositionsParams.ElementSize = sizeof(glm::vec2);

		mNGonPositions = OglBuffer(std::move(ngonPositionsParams));
		mNGonPositions.SetData(positions.data());
	}

	void Task1::UpdateNGonIndices()
	{
		switch (mCurrentSubtask)
		{
		case SubtaskName::A:
			LoadTriangles();
			break;
		case SubtaskName::B:
			LoadLines();
			break;
		case SubtaskName::C:
			LoadContour();
			break;
		case SubtaskName::D:
			LoadEvenLines();
			break;
		default:
			break;
		}

		OglGeometryInput::Params ngonGeometryParams;
		ngonGeometryParams.DebugName = "NGon geometry";
		ngonGeometryParams.VertexBuffers[0] = &mNGonPositions;
		ngonGeometryParams.IndexBuffer = &mNGonIndices;
		ngonGeometryParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};

		mNGonGeometry = OglGeometryInput(std::move(ngonGeometryParams));
	}

	void Task1::LoadTriangles()
	{
		unsigned int triangleIndices[]
		{
			4, 3, 2,
			2, 1, 0,
			5, 4, 0,
		};

		OglBuffer::Params triangleIndicesParams;
		triangleIndicesParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		triangleIndicesParams.DebugName = "Non-intersecting triangles indices";
		triangleIndicesParams.ElementSize = sizeof(unsigned int);
		triangleIndicesParams.ElementCount = std::size(triangleIndices);

		mNGonIndices = OglBuffer(std::move(triangleIndicesParams));
		mNGonIndices.SetData(triangleIndices);

		mNGonDrawMode = GL_TRIANGLES;
		mNGonPolygonMode = GL_LINE;
		mNGonDrawCount = std::size(triangleIndices);
	}

	void Task1::LoadLines()
	{
		unsigned int lineIndices[]
		{
			0, 2, 0, 3, 0, 4,
			1, 3, 1, 4, 1, 5,
			2, 4, 2, 5,
			3, 5,
		};

		OglBuffer::Params lineIndicesParams;
		lineIndicesParams.DebugName = "Intersecting lines indices";
		lineIndicesParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		lineIndicesParams.ElementSize = sizeof(unsigned int);
		lineIndicesParams.ElementCount = std::size(lineIndices);

		mNGonIndices = OglBuffer(std::move(lineIndicesParams));
		mNGonIndices.SetData(lineIndices);

		mNGonDrawMode = GL_LINES;
		mNGonPolygonMode = GL_LINE;
		mNGonDrawCount = std::size(lineIndices);
	}

	void Task1::LoadContour()
	{
		unsigned int contourIndices[]
		{
			0, 1, 2, 3, 4, 5,
		};

		OglBuffer::Params contourIndicesParams;
		contourIndicesParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		contourIndicesParams.DebugName = "Contour indices";
		contourIndicesParams.ElementSize = sizeof(unsigned int);
		contourIndicesParams.ElementCount = std::size(contourIndices);

		mNGonIndices = OglBuffer(std::move(contourIndicesParams));
		mNGonIndices.SetData(contourIndices);

		mNGonDrawMode = GL_LINE_LOOP;
		mNGonPolygonMode = GL_LINE;
		mNGonDrawCount = std::size(contourIndices);
	}

	void Task1::LoadEvenLines()
	{
		unsigned int evenLinesIndices[]
		{
			0, 2, 4,
		};

		OglBuffer::Params evenLinesIndicesParams;
		evenLinesIndicesParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		evenLinesIndicesParams.DebugName = "Event lines indices";
		evenLinesIndicesParams.ElementSize = sizeof(unsigned int);
		evenLinesIndicesParams.ElementCount = std::size(evenLinesIndices);

		mNGonIndices = OglBuffer(std::move(evenLinesIndicesParams));
		mNGonIndices.SetData(evenLinesIndices);

		mNGonDrawMode = GL_LINE_LOOP;
		mNGonPolygonMode = GL_LINE;
		mNGonDrawCount = std::size(evenLinesIndices);
	}

	void Task1::LoadShaders()
	{
		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "Task 1 shaders" });

		mPrograms.SetProgram(mShaders[ShaderStage::Vertex].FetchFromFile("glsl/basic_vert.glsl", "basic"));
		mPrograms.SetProgram(mShaders[ShaderStage::Fragment].FetchFromFile("glsl/randclr_frag.glsl", "randclr"));
	}
}

