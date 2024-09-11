#include "glabs/application/triangle_application.hpp"

#include "glabs/graphics/ogl_debug_output.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"

namespace glabs
{
	TriangleApplication::TriangleApplication()
	{
		mAppWindow.SetCloseCallback([this](){ mRunning = false; });

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(glabs::OglDebugMessageCallback, nullptr);

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
		mAppWindow.PollEvents();
	}

	void TriangleApplication::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		mPixelShader->SetUniform("tone", mTriangleTone[0], mTriangleTone[1], mTriangleTone[2]);

		mShaders->BindToPipeline();
		mTriangleInput->BindToPipeline();

		glViewport(0, 0, mAppWindow.GetWidth(), mAppWindow.GetHeight());
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		mAppWindow.Present();
	}

	void TriangleApplication::SetupResources()
	{
		glabs::OglBuffer::Params verticesBufferParams;
		verticesBufferParams.DebugName = "Triangle vertices buffer";
		verticesBufferParams.Target = GL_ARRAY_BUFFER;
		verticesBufferParams.ElementSize = sizeof(float[3]);
		verticesBufferParams.ElementCount = 3;

		mVerticesBuffer = std::make_unique<glabs::OglBuffer>(std::move(verticesBufferParams));
		mVerticesBuffer->SetData(mTriangleVertices);

		glabs::OglBuffer::Params colorBufferParams;
		colorBufferParams.DebugName = "Triangle color buffer";
		colorBufferParams.Target = GL_ARRAY_BUFFER;
		colorBufferParams.ElementSize = sizeof(float[3]);
		colorBufferParams.ElementCount = 3;

		mColorBuffer = std::make_unique<glabs::OglBuffer>(std::move(colorBufferParams));
		mColorBuffer->SetData(mTriangleVertexColors);

		glabs::OglBuffer::Params indexBufferParams;
		indexBufferParams.DebugName = "Triangle indices";
		indexBufferParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		indexBufferParams.ElementSize = sizeof(uint32_t);
		indexBufferParams.ElementCount = 3;

		mIndexBuffer = std::make_unique<glabs::OglBuffer>(std::move(indexBufferParams));
		mIndexBuffer->SetData(mTriangleIndices);

		glabs::OglGeometryInput::Params triangleGeometryInputParams;
		triangleGeometryInputParams.DebugName = "Triangle geometry input";
		triangleGeometryInputParams.Vertices = {
			glabs::VertexParams{ 0, glabs::VertexFormat::Float3 }, //< Vertex positions.
			glabs::VertexParams{ 1, glabs::VertexFormat::Float3 }, //< Vertex colors.
		};
		triangleGeometryInputParams.IndexBuffer = mIndexBuffer.get();
		triangleGeometryInputParams.VertexBuffers[0] = mVerticesBuffer.get();
		triangleGeometryInputParams.VertexBuffers[1] = mColorBuffer.get();

		mTriangleInput = std::make_unique<OglGeometryInput>(std::move(triangleGeometryInputParams));

		glabs::OglShaderProgram::Params vertexShaderParams;
		vertexShaderParams.DebugName = "Traingle vertex shader";
		vertexShaderParams.Stage = ShaderStage::Vertex;
		vertexShaderParams.Source =
R"(\
#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

out gl_PerVertex
{
	vec4 gl_Position;
};

out vec3 color;

void main()
{
	gl_Position = vec4(inPosition, 1.0f);
	color = inColor;
}
)";

		mVertexShader = std::make_unique<OglShaderProgram>(std::move(vertexShaderParams));

		glabs::OglShaderProgram::Params pixelShaderParams;
		pixelShaderParams.DebugName = "Traingle fragment shader";
		pixelShaderParams.Stage = ShaderStage::Fragment;
		pixelShaderParams.Source =
R"(\
#version 460 core

uniform vec3 tone;

in vec3 color;
out vec4 outColor;

void main()
{
	outColor = vec4(color * tone, 1.0f);
}
)";

		mPixelShader = std::make_unique<OglShaderProgram>(std::move(pixelShaderParams));

		glabs::OglProgramPipeline::Params shadersParams;
		shadersParams.DebugName = "Triangle shader pipeline";

		mShaders = std::make_unique<OglProgramPipeline>(std::move(shadersParams));
		mShaders->SetProgram(*mVertexShader);
		mShaders->SetProgram(*mPixelShader);
	}
}

