#include "glabs/lab3/task2.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace glabs
{
	Task2::Task2()
	{
		LoadShapes();
		LoadShaders();
	}

	TaskName Task2::GetName() const
	{
		return TaskName::Task2;
	}

	TaskName Task2::GetNext() const
	{
		return TaskName::Task3;
	}

	TaskName Task2::GetPrev() const
	{
		return TaskName::Task1;
	}

	std::string_view Task2::GetTitle() const
	{
		return "Задание 2";
	}

	void Task2::OnMainMenu()
	{
		ImGui::SliderFloat("Отдаление", &mZoom, 0.1f, 20.0f, "");
		ImGui::SameLine();
		if (ImGui::Button("Восстановить"))
		{
			mZoom = 1.0f;
		}
		ImGui::Checkbox("Показать линию", &mShowLine);
		ImGui::Checkbox("Показать треугольник", &mShowTriangle);
		ImGui::Checkbox("Показать прямоугольник", &mShowRect);
		ImGui::NewLine();
		ImGui::Checkbox("Линия", &mTransformLine);
		ImGui::Checkbox("Трегуольник", &mTransformTriangle);
		ImGui::Checkbox("Прямоугольник", &mTransformRect);
	}

	void Task2::OnRender()
	{
		mLineModel = mTransformLine
			? glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f))
			: glm::mat4(1.0f);

		mTriangleModel = mTransformTriangle
			? glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(-0.5f, 1.5f, 1.0f)), glm::vec3(1.0f, -0.5f, 0.0f))
			: glm::mat4(1.0f);

		mRectangleModel = mTransformRect
			? glm::translate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 3.0f, 0.0f)), glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f)), glm::vec3(10.0f, -3.0f, 0.0f))
			: glm::mat4(1.0f);

		mShapesGeometryInput.BindToPipeline();
		mPrograms.BindToPipeline();

		float orthoZoom = 1.0f * mZoom;
		mPrograms[ShaderStage::Vertex].Get().SetUniform("proj", glm::ortho(-orthoZoom, orthoZoom, -orthoZoom, orthoZoom), true);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (mShowLine)
		{
			mPrograms[ShaderStage::Vertex].Get().SetUniform("model", mLineModel, true);
			glDrawElements(
				GL_LINES,
				2,
				GL_UNSIGNED_INT,
				nullptr
			);
		}

		if (mShowTriangle)
		{
			mPrograms[ShaderStage::Vertex].Get().SetUniform("model", mTriangleModel, true);
			glDrawElements(
				GL_TRIANGLES,
				3,
				GL_UNSIGNED_INT,
				reinterpret_cast<void*>(2 * sizeof(unsigned int))
			);
		}

		if (mShowRect)
		{
			mPrograms[ShaderStage::Vertex].Get().SetUniform("model", mRectangleModel, true);
			glDrawElements(
				GL_TRIANGLES,
				6,
				GL_UNSIGNED_INT,
				reinterpret_cast<void*>(5 * sizeof(unsigned int))
			);
		}
	}

	void Task2::LoadShapes()
	{
		float shapePositions[]
		{
			// Line.
			-0.7f, 0.5f, 1.0f, 0.0f, 0.0f,
			 0.7f, 0.5f, 1.0f, 0.0f, 0.0f,

			// Triangle.
			 0.0f,  0.5f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.6f, 0.6f, 0.6f,

			// Rectangle.
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.6f, 0.4f,
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.6f, 0.0f, 1.0f,
		};

		unsigned int shapesIndices[]
		{
			// Line.
			0, 1,

			// Triangle.
			2, 3, 4,

			// Rectangle.
			5, 6, 7,
			7, 8, 5,
		};

		mShapesPositions = OglBuffer(OglBuffer::Params{
			"line, triangle, rect positions",
			GL_ARRAY_BUFFER,
			sizeof(float[5]),
			std::size(shapePositions) / 5
		});
		mShapesPositions.SetData(shapePositions);

		mShapesIndices = OglBuffer(OglBuffer::Params{
			"line, triangle, rect indices",
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int),
			std::size(shapesIndices)
		});
		mShapesIndices.SetData(shapesIndices);

		OglGeometryInput::Params shapesGeometryParams;
		shapesGeometryParams.DebugName = "Line, triangle, rectangle";
		shapesGeometryParams.VertexBuffers[0] = &mShapesPositions;
		shapesGeometryParams.IndexBuffer = &mShapesIndices;
		shapesGeometryParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }, //< Positions
			VertexParams{ 0, VertexFormat::Float3 } //< Colors
		};

		mShapesGeometryInput = OglGeometryInput(std::move(shapesGeometryParams));
	}

	void Task2::LoadShaders()
	{
		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "Line, triangle, rectangle programs" });

		mPrograms.SetProgram(mShaders[ShaderStage::Vertex].FetchFromFile("glsl/mvp_vert.glsl", "basic"));
		mPrograms.SetProgram(mShaders[ShaderStage::Fragment].FetchFromFile("glsl/mvp_frag.glsl", "basic"));
	}
}

