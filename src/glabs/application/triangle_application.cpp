#include "glabs/application/triangle_application.hpp"

#include "glabs/graphics/ogl_debug_output.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"

namespace glabs
{
	TriangleApplication::TriangleApplication()
	{
		std::cerr << glGetString(GL_VERSION) << '\n';

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
out vec3 position;

void main()
{
	position = inPosition;
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
in vec3 position;
out vec4 outColor;

float noise(vec2 co)
{
	return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	vec3 clampedColor = color;

	if (clampedColor.r >= 0.5f)
	{
		clampedColor = vec3(1.0f, 0.0f, 0.0f);
	}
	else if (clampedColor.g >= 0.5f)
	{
		clampedColor = vec3(0.0f, 1.0f, 0.0f);
	}
	else if (clampedColor.b >= 0.5f)
	{
		clampedColor = vec3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		float noiseResult = noise(position.xy);
		clampedColor = vec3(noiseResult, noiseResult, noiseResult) * tone;
	}

	outColor = vec4(clampedColor, 1.0f);
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
