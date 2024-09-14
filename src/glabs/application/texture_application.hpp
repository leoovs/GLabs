#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"
#include "glabs/graphics/window.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class TextureApplication
	{
	public:
		TextureApplication();

		void Run();

	private:
		void InitWindow();
		void InitRectange();
		void InitTexture();
		void InitShaders();

		void Update();
		void Render();

		Window mWindow;

		OglTexture2D mTexture;

		OglBuffer mRectanglePositions;
		OglBuffer mRectangleTexCoords;
		OglBuffer mRectangleIndexBuffer;
		OglGeometryInput mRectangleGeometry;

		ShaderLibrary mProgramLib;
		OglProgramPipeline mPrograms;

		bool mRunning = false;
	};
}

