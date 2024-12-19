#include "glabs/app/app_container.hpp"
#include "glabs/app/basic_app.hpp"
#include "glabs/graphics/glfw_life.hpp"
#include "glabs/graphics/image2d.hpp"
#include "glabs/graphics/ogl_cubemap.hpp"
#include "glabs/graphics/ogl_framebuffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/ogl_sampler.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"
#include "glabs/rendering/camera.hpp"
#include "glabs/rendering/spheric_camera_controller.hpp"
#include "glabs/rendering/mesh.hpp"
#include "glabs/rendering/obj_importer.hpp"
#include "glabs/rendering/shader_library.hpp"

#include <glm/ext/matrix_clip_space.hpp>

using namespace glabs;

class SkyboxApp : public BasicApp
{
private:
	void OnStart() override
	{
		LoadShaders();

		Window& window = GetWindow();
		OnWindowResize(window.GetWidth(), window.GetHeight());

		mStormtrooper = ObjImporter()
			.OpenFile("models/stormtrooper.obj")
			.LoadAllShapes()
			.Build();

		mCamera.SetEyePosition({ 0.0f, 0.0f, 3.0f });

		mCameraController.SetCamera(&mCamera);

		LoadCubemap();

		glfwSetInputMode(GetWindow().GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void OnUpdate(float dt) override
	{
		if (IsKeyDown(GLFW_KEY_W))
		{
			mCamera.Move(mCamera.GetFront() * dt);
		}
		if (IsKeyDown(GLFW_KEY_S))
		{
			mCamera.Move(-mCamera.GetFront() * dt);
		}
		if (IsKeyDown(GLFW_KEY_A))
		{
			mCamera.Move(-mCamera.GetRight() * dt);
		}
		if (IsKeyDown(GLFW_KEY_D))
		{
			mCamera.Move(mCamera.GetRight() * dt);
		}

		Render();
	}

	void OnWindowResize(int32_t width, int32_t height) override
	{
		mProjection = glm::perspective(glm::radians(60.0f), float(width) / height, 0.001f, 1000.0f);
		glViewport(0, 0, width, height);
	}

	void OnMouseMove(float x, float y) override
	{
		mCameraController.OnMouseMove(x, y);
	}

	void Render()
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 lookAtWithoutTranslation = glm::mat4(glm::mat3(mCamera.CalculateLookAt()));

		auto& vs = mPrograms[ShaderStage::Vertex].Get();
		vs.SetUniform("uViewProjection", mProjection * lookAtWithoutTranslation);

		mCubemap.BindToPipeline(0);
		mCubemapSampler.BindToPipeline(0);

		mCubeGeometry.BindToPipeline();
		mPrograms.BindToPipeline();

		glDrawArrays(GL_TRIANGLES, 0, 36);

		GetWindow().Present();
	}

	void LoadShaders()
	{
		auto& vs = mShaders[ShaderStage::Vertex].FetchFromFile("glsl/skybox_vert.glsl", "skybox");
		auto& ps = mShaders[ShaderStage::Fragment].FetchFromFile("glsl/skybox_frag.glsl", "skybox");

		mPrograms = OglProgramPipeline({ "Deferred Shading" });
		mPrograms[ShaderStage::Vertex].Set(vs);
		mPrograms[ShaderStage::Fragment].Set(ps);
	}

	void LoadCubemap()
	{
		float positions[]
		{
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		OglBuffer::Params cubeParams;
		cubeParams.DebugName = "Cube positions";
		cubeParams.Target = GL_ARRAY_BUFFER;
		cubeParams.ElementCount = 36;
		cubeParams.ElementSize = sizeof(float[3]);

		mCube = OglBuffer(std::move(cubeParams));
		mCube.SetData(positions);

		OglGeometryInput::Params cubeGeometryParams;
		cubeGeometryParams.DebugName = "Cube geometry";
		cubeGeometryParams.VertexBuffers[0] = &mCube;
		cubeGeometryParams.Vertices = { VertexParams{ 0, VertexFormat::Float3 } };

		mCubeGeometry = OglGeometryInput(std::move(cubeGeometryParams));

		OglCubemap::Params cubemapParams;
		cubemapParams.DebugName = "Cubemap";
		cubemapParams.SideSize = 2048;
		cubemapParams.Format = GraphicsFormat::R8G8B8_UNORM;
		cubemapParams.MipLevels = CalculateMipLevels(2048, 2048);

		mCubemap = OglCubemap(std::move(cubemapParams));

		auto faceNames = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };

		int iFace = 0;
		for (const char* faceName : faceNames)
		{
			Image2D face = Image2D::FromFile(std::string("models/") + faceName);
			mCubemap.SetData(face.GetPixels(), CubemapFace(iFace));
			iFace++;
		}

		mCubemap.GenerateMipMaps();

		OglSampler::Params cubemapSamplerParams;
		cubemapSamplerParams.DebugName = "Cubemap sampler";
		cubemapSamplerParams.MagnifyingFilter = TextureFilter::Linear;
		cubemapSamplerParams.MinimizingFilter = TextureFilter::Point;

		mCubemapSampler = OglSampler(std::move(cubemapSamplerParams));
	}

	Mesh mStormtrooper;
	Camera mCamera;
	SphericCameraController mCameraController;

	OglBuffer mCube;
	OglGeometryInput mCubeGeometry;

	ShaderLibrary mShaders;
	OglProgramPipeline mPrograms;

	OglCubemap mCubemap;
	OglSampler mCubemapSampler;

	glm::mat4 mProjection = glm::mat4(1.0f);
	glm::mat4 mModel = glm::mat4(1.0f);
};

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.Version = { 4, 6 };
	glfwParams.CoreProfile = true;
	glfwParams.EnableDebugContext = true;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::AppContainer()
			.Bind<SkyboxApp>()
			.Configure()
			.Run();
	}
}

