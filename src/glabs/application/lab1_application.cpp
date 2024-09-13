#include "glabs/application/lab1_application.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	Lab1Application::Lab1Application()
	{
		InitWindow();
		SetupGeometry();
		LoadShaders();
	}

	void Lab1Application::Run()
	{
		mRunning = true;

		while (mRunning)
		{
			Update();
			Render();
		}
	}

	void Lab1Application::InitWindow()
	{
		mAppWindow.SetCloseCallback([this](){ mRunning = false; });
	}

	void Lab1Application::SetupGeometry()
	{
		float triangles[9 * 3]
		{
			-0.9f, 0.2f, 0.0f,
			-0.55f, 0.9f, 0.0f,
			-0.2f, 0.2f, 0.0f,

			0.2f, 0.2f, 0.0f,
			0.55f, 0.9f, 0.0f,
			0.9f, 0.2f, 0.0f,

			-0.5f, -0.9f, 0.0f,
			0.0f, -0.15f, 0.0f,
			0.5f, -0.9f, 0.0f,
		};

		OglBuffer::Params trianglePositionsParams;
		trianglePositionsParams.DebugName = "Triangle positions";
		trianglePositionsParams.Target = GL_ARRAY_BUFFER;
		trianglePositionsParams.ElementSize = sizeof(float[3]);
		trianglePositionsParams.ElementCount = 9;

		mTrianglePositions = OglBuffer(std::move(trianglePositionsParams));
		mTrianglePositions.SetData(triangles);

		OglGeometryInput::Params triangleGeometryParams;
		triangleGeometryParams.DebugName = "Triangle geometry";
		triangleGeometryParams.VertexBuffers[0] = &mTrianglePositions;
		triangleGeometryParams.IndexBuffer = nullptr;
		triangleGeometryParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float3 }
		};

		mTriangleGeometry = OglGeometryInput(std::move(triangleGeometryParams));
	}

	void Lab1Application::LoadShaders()
	{
		mProgramLibrary[ShaderStage::Vertex].FetchFromFile("glsl/lab1_vertex.glsl", "lab1_vertex");
		mProgramLibrary[ShaderStage::Fragment].FetchFromFile("glsl/lab1_fragment1.glsl", "lab1_frag1");
		mProgramLibrary[ShaderStage::Fragment].FetchFromFile("glsl/lab1_fragment2.glsl", "lab1_frag2");
		mProgramLibrary[ShaderStage::Fragment].FetchFromFile("glsl/lab1_fragment3.glsl", "lab1_frag3");

		OglProgramPipeline::Params programsParams;
		programsParams.DebugName = "Triangle shader programs";

		mPrograms = OglProgramPipeline(std::move(programsParams));
	}

	void Lab1Application::Update()
	{
		mAppWindow.PollEvents();
	}

	void Lab1Application::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, mAppWindow.GetWidth(), mAppWindow.GetHeight());

		mTriangleGeometry.BindToPipeline();
		mPrograms.BindToPipeline();

		SetShader(ShaderStage::Vertex, "lab1_vertex");

		SetShader(ShaderStage::Fragment, "lab1_frag1");
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SetShader(ShaderStage::Fragment, "lab1_frag2");
		glDrawArrays(GL_TRIANGLES, 3, 3);

		SetShader(ShaderStage::Fragment, "lab1_frag3");
		glDrawArrays(GL_TRIANGLES, 6, 3);

		mAppWindow.Present();
	}

	void Lab1Application::SetShader(ShaderStage stage, const std::string& name)
	{
		mPrograms[stage].Set(mProgramLibrary[stage].FromName(name));
	}
}

