#include "glabs/lab4/task1.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"

namespace glabs
{
	Task1::Task1()
	{
		SetupShapes();
		LoadShaders();
	}

	void Task1::OnUI()
	{
		ImGui::Checkbox("Буфер глубины", &mEnableDepthTest);
		ImGui::Checkbox("Отобразить буфер глубины", &mDisplayDepthBuffer);
	}

	void Task1::OnRender()
	{
		(mEnableDepthTest ? glEnable : glDisable)(GL_DEPTH_TEST);
		mPrograms[ShaderStage::Fragment].Get().SetUniform("uImitateDepthBuffer", mDisplayDepthBuffer);

		mShapeVertexLayout.BindToPipeline();
		mPrograms.BindToPipeline();

		mPrograms[ShaderStage::Fragment].Get().SetUniform("uColor", 1.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		mPrograms[ShaderStage::Fragment].Get().SetUniform("uColor", 0.0f, 1.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 3, 6);

		mPrograms[ShaderStage::Fragment].Get().SetUniform("uColor", 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 7, 3);
	}

	TaskName Task1::GetName() const
	{
		return TaskName::Task1;
	}

	TaskName Task1::GetNext() const
	{
		return TaskName::None;
	}

	TaskName Task1::GetPrev() const
	{
		return TaskName::None;
	}

	void Task1::SetupShapes()
	{
		float shapeVertices[]
		{
			// First triangle.
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f,

			// Quad.
			 0.8f,  0.7f,  1.0f,
			-0.8f,  0.7f,  1.0f,
			-0.8f, -0.7f, -0.8f,
			-0.8f, -0.7f, -0.8f,
			 0.8f, -0.7f, -0.8f,
			 0.8f,  0.7f,  1.0f,

			// Second triangle.
			 0.0f, 0.5f, -0.5f,
			-0.5f, 0.0f, -0.5f,
			 0.5f, 0.0f,  0.5f,
		};

		mShapeVertexBuffer = OglBuffer(OglBuffer::Params{
			"Shape vertex buffer",
			GL_ARRAY_BUFFER,
			sizeof(float[3]),
			10
		});
		mShapeVertexBuffer.SetData(shapeVertices);

		OglGeometryInput::Params shapeVertexLayoutParams;
		shapeVertexLayoutParams.DebugName = "Shape vertex layout";
		shapeVertexLayoutParams.VertexBuffers[0] = &mShapeVertexBuffer;
		shapeVertexLayoutParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float3 }
		};

		mShapeVertexLayout = OglGeometryInput(std::move(shapeVertexLayoutParams));
	}

	void Task1::LoadShaders()
	{
		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "shape shaders" });

		mPrograms.SetProgram(
			mShaders[ShaderStage::Vertex]
				.FetchFromFile("glsl/flat_vert.glsl", "flat")
		);

		mPrograms.SetProgram(
			mShaders[ShaderStage::Fragment]
				.FetchFromFile("glsl/flat_frag.glsl", "flat")
		);
	}
}
