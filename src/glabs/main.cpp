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
#include "glabs/rendering/sub_mesh.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class ModelViewer : public glabs::BasicApp
{
private:
	void OnStart() override
	{
		mModel = glabs::ObjImporter()
			.OpenFile("models/sphere.obj")
			.LoadAllShapes()
			.Build();

		mPrograms = glabs::OglProgramPipeline({glabs::OglProgramPipeline::Params{ "Model viewer programs" }});

		mPrograms.SetProgram(
			mShaders[glabs::ShaderStage::Vertex]
				.FetchFromFile("glsl/viewer_vert.glsl", "viewer")
		);
		mPrograms.SetProgram(
			mShaders[glabs::ShaderStage::Fragment]
				.FetchFromFile("glsl/viewer_frag.glsl", "viewer")
		);

		mSceneColor = glabs::OglTexture2D({
			"Scene color",

			1280,
			720,
			1,
			glabs::GraphicsFormat::R8G8B8A8_UNORM,
		});

		mSceneDepth = glabs::OglTexture2D({
			"Scene depth",

			1280,
			720,
			1,
			glabs::GraphicsFormat::D32_UNORM
		});

		mSceneFb = glabs::OglFramebuffer({ "Scene" });
		mSceneFb.SetAttachment(glabs::OglFramebuffer::Attachment::Color, mSceneColor);
		mSceneFb.SetAttachment(glabs::OglFramebuffer::Attachment::DepthStencil, mSceneDepth);

		glEnable(GL_DEPTH_TEST);
	}

	void OnUpdate(float dt) override
	{
		GetImGui().NewFrame();
		{
			if (ImGui::Begin("Scene"))
			{
				ImVec2 windowSize = ImGui::GetWindowContentRegionMax();
				windowSize.y -= 40.0f;
				ImTextureID image = GetImGui().GetTextureID(mSceneColor);
				ImGui::Image(image, windowSize, { 0, 1 }, { 1, 0 });

				mDisplayRatio = windowSize.x / windowSize.y;
			}
			ImGui::End();

			if (ImGui::Begin("Sphere"))
			{
				ImGui::DragFloat("Rotation OY", &mRotationY, 0.1f, 0.0f, glm::two_pi<float>(), "%.1f", ImGuiSliderFlags_WrapAround);
				ImGui::DragFloat2("Offset OZ OX", &mOffsetZ, 0.1f, -10.0f, 10.0f);
			}
			ImGui::End();
		}
		ImGui::Render();

		Render();
	}

	void Render()
	{
		mSceneFb.BindToPipeline();
		mSceneFb.ClearColor({0.5f, 0.0f, 0.2f, 1.0f});
		mSceneFb.ClearDepth(1.0f);
		mPrograms.BindToPipeline();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), { mOffsetX, 0.0f, mOffsetZ });
		model = glm::rotate(model, mRotationY, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(45.0f, mDisplayRatio, 0.001f, 100.0f);

		auto& vertexShader = mPrograms[glabs::ShaderStage::Vertex].Get();
		vertexShader.SetUniform(
			"uInvTranspModel",
			model
		);
		vertexShader.SetUniform(
			"uViewProjection",
			projection * mCamera.LookAt({ 0.0f, 0.0f, 4.0f })
		);

		mModel.ForEachShape(
			[](const glabs::Submesh& shape)
			{
				shape.GetGeometry().BindToPipeline();

				size_t verticesToDraw = shape.GetVertexCount();
				glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);
			}
		);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		GetImGui().RenderDrawData(ImGui::GetDrawData());
		GetWindow().Present();
	}

private:
	glabs::Mesh mModel;

	glabs::OglProgramPipeline mPrograms;
	glabs::ShaderLibrary mShaders;

	glabs::OglTexture2D mSceneColor;
	glabs::OglTexture2D mSceneDepth;
	glabs::OglFramebuffer mSceneFb;

	glabs::Camera mCamera;
	float mDisplayRatio = 1.0f;
	float mOffsetZ = 0.0f;
	float mOffsetX = 0.0f;
	float mRotationY = 0.0f;
};

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.Version = { 4, 6 };
	glfwParams.CoreProfile = true;
	glfwParams.EnableDebugContext = true;

	int a;
	std::cout << a << std::endl;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::AppContainer()
			.Bind<ModelViewer>()
			.Configure()
			.Run();
	}
}

