#include "glabs/lab4/task3.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glabs
{
	Task3::Task3()
	{
		SetupCube();
		SetupProjections();
		LoadShaders();
	}

	void Task3::OnUI()
	{
		bool projectionChanged = ImGui::Combo(
			u8"Проекция",
			reinterpret_cast<int*>(&mProjection),
			[](void* userdata, int index)
			{
				return ProjectionToStr(static_cast<Projection>(index)).data();
			},
			nullptr,
			static_cast<size_t>(Projection::Count_)
		);

		if (projectionChanged)
		{
			if (mProjection == Projection::Ortho)
			{
				mCurrentProjection = &mOrthoProjection;
			}
			else
			{
				mCurrentProjection = &mPerspectiveProjection;
			}
		}

		ImGui::NewLine();
		ImGui::Checkbox("Каркасное отображение", &mCubeWireframe);
		ImGui::NewLine();

		ImGui::DragFloat3("Смещение", glm::value_ptr(mCubeTranslation), 0.05f, -20.0f, 20.0f);
		ImGui::DragFloat3("Поворот", glm::value_ptr(mCubeRotation), 0.05f, 0.0f, glm::two_pi<float>(), "%.1f", ImGuiSliderFlags_WrapAround);
	}

	void Task3::OnRender()
	{
		glEnable(GL_DEPTH_TEST);

		mModel = glm::translate(glm::mat4(1.0f), mCubeTranslation);
		mModel = glm::rotate(mModel, mCubeRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		mModel = glm::rotate(mModel, mCubeRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		mModel = glm::rotate(mModel, mCubeRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		mCubePrograms.BindToPipeline();
		mCubeGeometryInput.BindToPipeline();

		auto& vertexShader = mCubePrograms[ShaderStage::Vertex].Get();
		vertexShader.SetUniform("uModel", mModel);
		vertexShader.SetUniform("uProj", *mCurrentProjection);

		auto& fragmentShader = mCubePrograms[ShaderStage::Fragment].Get();
		fragmentShader.SetUniform("uDrawWireframe", mCubeWireframe);

		glPolygonMode(GL_FRONT_AND_BACK, mCubeWireframe ? GL_LINE : GL_FILL);

		glDrawElements(GL_TRIANGLES, mCubeIndexBuffer.GetParams().ElementCount, GL_UNSIGNED_INT, nullptr);
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
		return TaskName::Task1;
	}

	void Task3::SetupProjections()
	{
		mOrthoProjection = glm::ortho(
			-0.85f,
			0.85f,
			-0.7f,
			0.7f,
			6.0f, 12.0f
		);

		GLint viewport[4]{};
		glGetIntegerv(GL_VIEWPORT, viewport);

		mPerspectiveProjection = glm::perspective(
			glm::radians(45.0f),
			float(viewport[2]) / viewport[3],
			0.01f, 15.0f
		);
	}

	void Task3::SetupCube()
	{
		float positions[]
		{
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f
		};

		for (float& component : positions)
		{
			component *= glm::sqrt(3.0f) / 3.0f;
		}

		mCubeVertexBuffer = OglBuffer(OglBuffer::Params{
			"cube vertex buffer",
			GL_ARRAY_BUFFER,
			sizeof(float[3]),
			8
		});
		mCubeVertexBuffer.SetData(positions);

		unsigned int indices[]
		{
			0, 1, 2, 2, 3, 0, // front
			0, 4, 5, 5, 1, 0, // right
			4, 7, 6, 6, 5, 4, // back
			7, 3, 2, 2, 6, 7, // left
			0, 3, 7, 7, 4, 0, // bottom
			1, 5, 6, 6, 2, 1  // top
		};

		mCubeIndexBuffer = OglBuffer(OglBuffer::Params{
			"cube index buffer",
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int),
			std::size(indices)
		});
		mCubeIndexBuffer.SetData(indices);

		OglGeometryInput::Params cubeGeometryInputParams;
		cubeGeometryInputParams.DebugName = "cube geometry";
		cubeGeometryInputParams.VertexBuffers[0] = &mCubeVertexBuffer;
		cubeGeometryInputParams.IndexBuffer = &mCubeIndexBuffer;
		cubeGeometryInputParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float3 }
		};

		mCubeGeometryInput = OglGeometryInput(std::move(cubeGeometryInputParams));
	}

	void Task3::LoadShaders()
	{
		mCubePrograms = OglProgramPipeline(OglProgramPipeline::Params{ "cube shaders" });

		mCubePrograms.SetProgram(
			mShaders[ShaderStage::Vertex]
				.FetchFromFile("glsl/cube_vert.glsl", "cube")
		);
		mCubePrograms.SetProgram(
			mShaders[ShaderStage::Fragment]
				.FetchFromFile("glsl/cube_frag.glsl", "cube")
		);
	}
}

