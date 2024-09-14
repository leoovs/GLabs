#include "glabs/application/triangle_application.hpp"

#include "glabs/graphics/ogl_debug_output.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"

namespace glabs
{
	TriangleApplication::TriangleApplication()
	{
		std::cerr << glGetString(GL_VERSION) << '\n';

		mAppWindow = Window(1280, 720, "Hello triangle");
		mAppWindow.SetCloseCallback([this](){ mRunning = false; });

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(OglDebugMessageCallback, nullptr);

		DearImGui::Params imguiParams;
		imguiParams.OutputWindow = &mAppWindow;
		mImGui = DearImGui(std::move(imguiParams));

		SetupResources();
	}

	void TriangleApplication::Run()
	{
		mRunning = true;

		while (mRunning)
		{
			Update();
			Render();
		}
	}

	void TriangleApplication::Update()
	{
		mImGui.NewFrame();
		mAppWindow.PollEvents();

		ImGui::ShowDemoWindow();
		ImGui::Render();
	}

	void TriangleApplication::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		mShaders[ShaderStage::Fragment].Get().SetUniform("tone", mTriangleTone[0], mTriangleTone[1], mTriangleTone[2]);

		mShaders.BindToPipeline();
		mTriangleInput.BindToPipeline();

		glViewport(0, 0, mAppWindow.GetWidth(), mAppWindow.GetHeight());
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		mImGui.RenderDrawData(ImGui::GetDrawData());

		mAppWindow.Present();
	}

	void TriangleApplication::SetupResources()
	{
		OglBuffer::Params verticesBufferParams;
		verticesBufferParams.DebugName = "Triangle vertices buffer";
		verticesBufferParams.Target = GL_ARRAY_BUFFER;
		verticesBufferParams.ElementSize = sizeof(float[3]);
		verticesBufferParams.ElementCount = 3;

		mVerticesBuffer = OglBuffer(std::move(verticesBufferParams));
		mVerticesBuffer.SetData(mTriangleVertices);

		OglBuffer::Params colorBufferParams;
		colorBufferParams.DebugName = "Triangle color buffer";
		colorBufferParams.Target = GL_ARRAY_BUFFER;
		colorBufferParams.ElementSize = sizeof(float[3]);
		colorBufferParams.ElementCount = 3;

		mColorBuffer = OglBuffer(std::move(colorBufferParams));
		mColorBuffer.SetData(mTriangleVertexColors);

		OglBuffer::Params indexBufferParams;
		indexBufferParams.DebugName = "Triangle indices";
		indexBufferParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		indexBufferParams.ElementSize = sizeof(uint32_t);
		indexBufferParams.ElementCount = 3;

		mIndexBuffer = OglBuffer(std::move(indexBufferParams));
		mIndexBuffer.SetData(mTriangleIndices);

		OglGeometryInput::Params triangleGeometryInputParams;
		triangleGeometryInputParams.DebugName = "Triangle geometry input";
		triangleGeometryInputParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float3 }, //< Vertex positions.
			VertexParams{ 1, VertexFormat::Float3 }, //< Vertex colors.
		};
		triangleGeometryInputParams.IndexBuffer = &mIndexBuffer;
		triangleGeometryInputParams.VertexBuffers[0] = &mVerticesBuffer;
		triangleGeometryInputParams.VertexBuffers[1] = &mColorBuffer;

		mTriangleInput = OglGeometryInput(std::move(triangleGeometryInputParams));

		OglProgramPipeline::Params shadersParams;
		shadersParams.DebugName = "Triangle shader pipeline";

		mShaders = OglProgramPipeline(std::move(shadersParams));

		mShaders[ShaderStage::Vertex].Set(
			mShaderLib[ShaderStage::Vertex]
				.FetchFromFile("glsl/triangle_vertex.glsl", "triangle-vertex")
		);
		mShaders[ShaderStage::Fragment].Set(
			mShaderLib[ShaderStage::Fragment]
				.FetchFromFile("glsl/triangle_fragment.glsl", "triangle-fragment")
		);
	}
}

