#include "glabs/app/app_container.hpp"
#include "glabs/app/basic_app.hpp"
#include "glabs/graphics/glfw_life.hpp"
#include "glabs/graphics/ogl_framebuffer.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"
#include "glabs/rendering/camera.hpp"
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
		mCamera.LookAt({ 0.0f, 2.0f, 0.0f });
	}

	void OnUpdate(float dt) override
	{
		Render();
	}

	void OnWindowResize(int32_t width, int32_t height) override
	{
		SetupDeferredPass(width, height);
		mProjection = glm::perspective(glm::radians(45.0f), float(width) / height, 0.001f, 1000.0f);
	}

	void Render()
	{
		glEnable(GL_DEPTH_TEST);
		mDeferredPass.ClearColor(glm::vec4(0.0f), 0);
		mDeferredPass.ClearColor(glm::vec4(0.0f), 1);
		mDeferredPass.ClearColor(glm::vec4(0.0f), 2);
		mDeferredPass.ClearDepth(1.0f);
		mDeferredPass.BindToPipeline();

		auto& vs = mPrograms[ShaderStage::Vertex].Get();
		vs.SetUniform("uViewProjection", mProjection * mCamera.CalculateLookAt());
		vs.SetUniform("uModel", mModel);
		vs.SetUniform("uInversedTransposedModel", glm::transpose(glm::inverse(mModel)));

		mPrograms.BindToPipeline();

		mStormtrooper.ForEachShape(
			[this](const Submesh& shape)
			{
				shape.GetGeometry().BindToPipeline();
				glDrawArrays(GL_TRIANGLES, 0, shape.GetVertexCount());
			}
		);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GetWindow().Present();
	}

	void LoadShaders()
	{
		auto& vs = mShaders[ShaderStage::Vertex].FetchFromFile("glsl/def_vert.glsl", "def");
		auto& ps = mShaders[ShaderStage::Fragment].FetchFromFile("glsl/def_frag.glsl", "def");

		mPrograms = OglProgramPipeline({ "Deferred Shading" });
		mPrograms[ShaderStage::Vertex].Set(vs);
		mPrograms[ShaderStage::Fragment].Set(ps);
	}

	void SetupDeferredPass(int32_t width, int32_t height)
	{
		OglTexture2D::Params attribTextureParams;
		attribTextureParams.DebugName = "Positions texture";
		attribTextureParams.Width = width;
		attribTextureParams.Height = height;
		attribTextureParams.Format = GraphicsFormat::R16G16B16_FLOAT;
		attribTextureParams.MipLevels = OglTexture2D::CalculateMipLevels(width, height);

		mPositionsTexture = OglTexture2D(attribTextureParams);

		attribTextureParams.DebugName = "Normals texture";

		mNormalsTexture = OglTexture2D(attribTextureParams);

		attribTextureParams.DebugName = "UV texture";
		attribTextureParams.Format = GraphicsFormat::R16G16_FLOAT;

		mUVTexture = OglTexture2D(std::move(attribTextureParams));

		OglTexture2D::Params depthBufferParams;
		depthBufferParams.DebugName = "Deferred depth buffer";
		depthBufferParams.Width = width;
		depthBufferParams.Height = width;
		depthBufferParams.Format = GraphicsFormat::D32_UNORM;
		depthBufferParams.MipLevels = OglTexture2D::CalculateMipLevels(width, height);

		mDepthBuffer = OglTexture2D(std::move(depthBufferParams));

		mDeferredPass = OglFramebuffer({ "Deferred pass" });
		mDeferredPass.SetAttachment(OglFramebuffer::Attachment::Color0, mPositionsTexture);
		mDeferredPass.SetAttachment(OglFramebuffer::Attachment::Color1, mNormalsTexture);
		mDeferredPass.SetAttachment(OglFramebuffer::Attachment::Color2, mUVTexture);
		mDeferredPass.SetAttachment(OglFramebuffer::Attachment::DepthStencil, mDepthBuffer);
	}

	Mesh mStormtrooper;
	Camera mCamera;

	ShaderLibrary mShaders;
	OglProgramPipeline mPrograms;

	OglFramebuffer mDeferredPass;
	OglTexture2D mPositionsTexture;
	OglTexture2D mNormalsTexture;
	OglTexture2D mUVTexture;
	OglTexture2D mDepthBuffer;

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

