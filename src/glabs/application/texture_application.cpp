#include "glabs/application/texture_application.hpp"

#include "glabs/graphics/image2d.hpp"
#include "glabs/graphics/ogl_debug_output.hpp"

namespace glabs
{
	TextureApplication::TextureApplication()
	{
		InitWindow();
		InitRectange();
		InitTexture();
		InitShaders();
	}

	void TextureApplication::Run()
	{
		mRunning = true;

		while (mRunning)
		{
			Update();
			Render();
		}
	}

	void TextureApplication::InitWindow()
	{
		mWindow = Window(1280, 720, "Texture application");

		mWindow.SetCloseCallback([this](){ mRunning = false; });

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(OglDebugMessageCallback, nullptr);
	}

	void TextureApplication::InitRectange()
	{
		float rectPositions[]
		{
			0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
		};

		float rectTexCoords[]
		{
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
		};

		uint32_t rectIndicies[]{ 0, 1, 3, 1, 2, 3 };

		mRectanglePositions = OglBuffer(OglBuffer::Params{
			"Rectangle positions",
			GL_ARRAY_BUFFER,
			sizeof(float[3]),
			4
		});
		mRectanglePositions.SetData(rectPositions);

		mRectangleTexCoords = OglBuffer(OglBuffer::Params{
			"Rectangle positions",
			GL_ARRAY_BUFFER,
			sizeof(float[2]),
			4
		});
		mRectangleTexCoords.SetData(rectTexCoords);

		mRectangleIndexBuffer = OglBuffer(OglBuffer::Params{
			"Rectangle index buffer",
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(uint32_t),
			6
		});
		mRectangleIndexBuffer.SetData(rectIndicies);

		OglGeometryInput::Params rectGeometryParams;
		rectGeometryParams.DebugName = "Rectangle geometry";
		rectGeometryParams.VertexBuffers[0] = &mRectanglePositions;
		rectGeometryParams.VertexBuffers[1] = &mRectangleTexCoords;
		rectGeometryParams.IndexBuffer = &mRectangleIndexBuffer;
		rectGeometryParams.Vertices = {
			VertexParams{ 0, VertexFormat::Float3 },
			VertexParams{ 1, VertexFormat::Float2 },
		};
		mRectangleGeometry = OglGeometryInput(std::move(rectGeometryParams));
	}

	void TextureApplication::InitTexture()
	{
		Image2D image = Image2D::FromFile("texture_asset.png", 4);

		mTexture = OglTexture2D(OglTexture2D::Params{
			"Test texture",
			image.GetWidth(),
			image.GetHeight(),
			OglTexture2D::CalculateMipLevels(image.GetWidth(), image.GetHeight()),
			GraphicsFormat::R8G8B8A8_UNORM
		});
		mTexture.SetData(image.GetPixels());
		mTexture.GenerateMipMaps();
	}

	void TextureApplication::InitShaders()
	{
		mProgramLib[ShaderStage::Vertex].FetchFromFile("glsl/texture_vertex.glsl", "vert");
		mProgramLib[ShaderStage::Fragment].FetchFromFile("glsl/texture_fragment.glsl", "frag");

		mPrograms = OglProgramPipeline(OglProgramPipeline::Params{ "Rectangle shaders" });
	}

	void TextureApplication::Update()
	{
		mWindow.PollEvents();
	}

	void TextureApplication::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, mWindow.GetWidth(), mWindow.GetHeight());

		mPrograms[ShaderStage::Vertex].Set(mProgramLib[ShaderStage::Vertex].FromName("vert"));
		mPrograms[ShaderStage::Fragment].Set(mProgramLib[ShaderStage::Fragment].FromName("frag"));

		mTexture.BindToPipeline(0);
		mRectangleGeometry.BindToPipeline();
		mPrograms.BindToPipeline();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		mWindow.Present();
	}
}

