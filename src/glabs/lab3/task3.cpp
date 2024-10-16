#include "glabs/lab3/task3.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "imgui.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace glabs
{
	Task3::Task3()
	{
		LoadShape();
		LoadShaders();
	}

	TaskName Task3::GetName() const
	{
		return TaskName::Task3;
	}

	TaskName Task3::GetNext() const
	{
		return TaskName::Task1;
	}

	TaskName Task3::GetPrev() const
	{
		return TaskName::Task2;
	}

	std::string_view Task3::GetTitle() const
	{
		return "Задание 3";
	}

	void Task3::OnMainMenu()
	{
		ImGui::Text("%s", "Сдвиг");
		ImGui::PushItemWidth(100.0f);
		ImGui::DragFloat("x##TranslationX", &mTranslation.x, 0.1f,-10.0f, 10.0f);
		ImGui::SameLine();
		ImGui::DragFloat("y##TranslationY", &mTranslation.y, 0.1f,-10.0f, 10.0f);
		ImGui::PopItemWidth();

		ImGui::Text("%s", "Масштабирование");
		ImGui::PushItemWidth(100.0f);
		ImGui::DragFloat("x##ScaleX", &mScale.x, 0.1f, -10.0f, 10.0f);
		ImGui::SameLine();
		ImGui::DragFloat("y##ScaleY", &mScale.y, 0.1f, -10.0f, 10.0f);
		ImGui::PopItemWidth();

		ImGui::Text("%s", "Поворот");
		ImGui::DragFloat("##Rotation", &mRotation, 1.0f, 0.0f, 360.0f, "%.2f", ImGuiSliderFlags_WrapAround);

		ImGui::NewLine();

		ImGui::SliderFloat("Отдаление", &mZoom, 1.0f, 100.0f, "");
	}

	void Task3::OnRender()
	{
		float ortho = mZoom;
		glm::mat4 proj = glm::ortho(-ortho, ortho, -ortho, ortho);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), mTranslation);
		model = glm::rotate(model, glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, mScale);

		OglShaderProgram& vs = mPrograms[ShaderStage::Vertex].Get();
		vs.SetUniform("proj", proj);
		vs.SetUniform("model", model);

		mPrograms.BindToPipeline();
		mGeometry.BindToPipeline();

		glDrawElements(GL_LINE_STRIP, 13, GL_UNSIGNED_INT, nullptr);
	}

	void Task3::LoadShape()
	{
		float positions[]
		{
			0.0f, 0.0f,
			0.5f, -0.5f,
			0.3f, -0.7f,
			-0.3f, -0.7f,
			-0.5f, -0.5f,
			-0.8f, -0.0f,
			-0.8f, 0.8f,
			0.8f, 0.8f,
			0.8f, 0.0f,
		};

		unsigned int indices[]
		{
			0, 1, 2, 0,
			3, 4, 0,
			5, 6, 0,
			7, 8, 0,
		};

		mPositions = OglBuffer(OglBuffer::Params{
			"Shape positions",
			GL_ARRAY_BUFFER,
			sizeof(float[2]),
			std::size(positions) / 2
		});
		mPositions.SetData(positions);

		mIndices = OglBuffer(OglBuffer::Params{
			"Shape indices",
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int),
			std::size(indices)
		});
		mIndices.SetData(indices);

		OglGeometryInput::Params shapeGeometryParams;
		shapeGeometryParams.DebugName = "Shape geometry";
		shapeGeometryParams.VertexBuffers[0] = &mPositions;
		shapeGeometryParams.IndexBuffer = &mIndices;
		shapeGeometryParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};
		mGeometry = OglGeometryInput(std::move(shapeGeometryParams));
	}

	void Task3::LoadShaders()
	{
		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "Shape shaders" });

		mPrograms.SetProgram(mShaders[ShaderStage::Vertex].FetchFromFile("glsl/mvp_vert.glsl", "shape"));
		mPrograms.SetProgram(mShaders[ShaderStage::Fragment].FetchFromFile("glsl/randclr_frag.glsl", "shape"));
	}
}

