#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/window.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class TriangleApplication
	{
	public:
		TriangleApplication();

		void Run();

	private:
		void Update();
		void Render();

		void SetupResources();

		bool mRunning = false;

		Window mAppWindow;

		OglBuffer mVerticesBuffer;
		OglBuffer mColorBuffer;
		OglBuffer mIndexBuffer;
		OglGeometryInput mTriangleInput;
		OglProgramPipeline mShaders;
		ShaderLibrary mShaderLib;

		float mTriangleVertices[9]
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};

		float mTriangleVertexColors[9]
		{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		};

		float mTriangleTone[3]{ 1.0f, 0.5f, 1.0f };

		uint32_t mTriangleIndices[3]{ 0, 1, 2 };
	};
}

