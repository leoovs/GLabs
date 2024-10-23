#include "glabs/graphics/glfw_life.hpp"
#include "glabs/app/app_container.hpp"
#include "glabs/app/basic_app.hpp"
#include "glabs/graphics/ogl_framebuffer.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"
#include "glabs/pch.hpp"
#include "glabs/rendering/dear_imgui.hpp"
#include "glabs/rendering/mesh.hpp"
#include "glabs/rendering/obj_importer.hpp"
#include "glabs/rendering/shader_library.hpp"
#include "glabs/rendering/camera.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace glabs;

struct Entity
{
	glm::vec3 Translation = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::mat4 CalculateMatrix() const
	{
		auto model = glm::identity<glm::mat4>();

		model = glm::scale(model, Scale);
		model = glm::rotate(model, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		return model = glm::translate(model, Translation);
	}
};

class TestApp : public BasicApp
{
private:
	std::string_view GetName() const override
	{
		return "O-o";
	}

	void OnStart() override
	{
		mModel = ObjImporter()
			.OpenFile("glsl/sphere.obj")
			.LoadAllShapes()
			.Build();

		mPrograms = OglProgramPipeline({ "shaders" });

		mPrograms.SetProgram(mShaders[ShaderStage::Vertex]
			.FetchFromFile("glsl/model_vert.glsl", "model"));
		mPrograms.SetProgram(mShaders[ShaderStage::Fragment]
			.FetchFromFile("glsl/model_frag.glsl", "model"));

		mModelEntity.Translation = { 0.0f, 0.0f, 0.0f };
		mCameraEntity.Translation = { 0.0f, 0.0f, 0.0f };

		OglFramebuffer::Params sceneBufferParams;
		sceneBufferParams.DebugName = "Scene fb";

		sceneBufferParams.ColorAttachmentParams.DebugName = "Scene fb color attachment";
		sceneBufferParams.ColorAttachmentParams.Width = 720;
		sceneBufferParams.ColorAttachmentParams.Height = 720 * 9 / 16;
		sceneBufferParams.ColorAttachmentParams.Format = GraphicsFormat::R8G8B8A8_UNORM;
		sceneBufferParams.ColorAttachmentParams.MipLevels = 1;
		sceneBufferParams.ColorAttachmentMipMapIndex = 0;

		sceneBufferParams.DepthAttachmentParams.DebugName = "Scene fb depth attachment";
		sceneBufferParams.DepthAttachmentParams.Width = 720;
		sceneBufferParams.DepthAttachmentParams.Height = 720 * 9 / 16;
		sceneBufferParams.DepthAttachmentParams.Format = GraphicsFormat::D32_UNORM;
		sceneBufferParams.DepthAttachmentParams.MipLevels = 1;
		sceneBufferParams.DepthAttachmentMipMapIndex = 0;

		mSceneBuffer = OglFramebuffer(std::move(sceneBufferParams));
	}

	void OnWindowResize(int32_t width, int32_t height) override
	{
		Render();
	}

	void ShowUserInterface()
	{
		GetImGui().NewFrame();

		ImGui::Begin("Scene manager");

		int state = glfwGetMouseButton(GetWindow().GetNativeWindow(), GLFW_MOUSE_BUTTON_LEFT);
		if (ImGui::IsWindowHovered() && GLFW_PRESS == state)
		{
			mIsFocused = true;
			glfwSetInputMode(GetWindow().GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		ImGui::Image(
			GetImGui().GetTextureID(mSceneBuffer.GetColorAttachment()),
			{
				float(mSceneBuffer.GetParams().ColorAttachmentParams.Width),
				float(mSceneBuffer.GetParams().ColorAttachmentParams.Height),
			},
			{ 0, 1 },
			{ 1, 0 }
		);

		ImGui::End();

		ImGui::Render();
	}

	void OnUpdate(float dt) override
	{
		ShowUserInterface();

		if (mIsFocused)
		{
			ProcessInput(dt);
		}

		Render();
	}

	void ProcessInput(float dt)
	{
		float speed = 2.0f * dt;
		glm::vec3 movement(0.0f, 0.0f, 0.0f);

		if (IsKeyDown(GLFW_KEY_D))
		{
			movement += mCam.GetRight();
			movement.y = 0.0f;
		}
		if (IsKeyDown(GLFW_KEY_A))
		{
			movement -= mCam.GetRight();
			movement.y = 0.0f;
		}
		if (IsKeyDown(GLFW_KEY_W))
		{
			movement += mCam.GetFront();
			movement.y = 0.0f;
		}
		if (IsKeyDown(GLFW_KEY_S))
		{
			movement -= mCam.GetFront();
			movement.y = 0.0f;
		}
		if (IsKeyDown(GLFW_KEY_SPACE))
		{
			movement.y += 1.0f;
		}
		if (IsKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			movement.y -= 1.0f;
		}
		if (IsKeyDown(GLFW_KEY_ESCAPE))
		{
			mIsFocused = false;
			glfwSetInputMode(GetWindow().GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		}

		if (glm::length(movement) > glm::epsilon<float>())
		{
			movement = glm::normalize(movement);
		}

		mCameraEntity.Translation += movement * speed;
	}

	void Render()
	{
		mSceneBuffer.BindToPipeline();
		const OglFramebuffer::Params& sceneBufferParams = mSceneBuffer.GetParams();

		int32_t width = sceneBufferParams.ColorAttachmentParams.Width;
		int32_t height = sceneBufferParams.ColorAttachmentParams.Height;

		float aspect = width / float(height);

		glm::mat4 proj = glm::perspective(glm::radians(60.0f), aspect, 0.01f, 20.0f);
		glm::mat4 view = mCam.LookAt(mCameraEntity.Translation);
		glm::mat4 model = mModelEntity.CalculateMatrix();
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

		mSceneBuffer.ClearColor({ 0.0f, 0.0f, 0.0f, 1 });
		mSceneBuffer.ClearDepth(1.0f);
		glViewport(0, 0, width, height);

		mPrograms.BindToPipeline();
		auto& vertexShader = mPrograms[ShaderStage::Vertex].Get();
		vertexShader.SetUniform("uModelMtx", model);
		vertexShader.SetUniform("uNormalMtx", normalMatrix);
		vertexShader.SetUniform("uViewProjectionMtx", proj * view);

		mModel.ForEachShape(
			[](const Submesh& shape)
			{
				glEnable(GL_CULL_FACE);
				glEnable(GL_DEPTH_TEST);
				glCullFace(GL_BACK);
				shape.GetGeometry().BindToPipeline();
				glDrawArrays(GL_TRIANGLES, 0, shape.GetVertexCount());
			}
		);

		OglFramebuffer::BindDefaultToPipeline();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GetImGui().RenderDrawData(ImGui::GetDrawData());

		GetWindow().Present();
	}

	void OnMouseMove(float x, float y) override
	{
		constexpr float cSensitivity = 0.004f;

		static float sX = x;
		static float sY = y;

		float dx = x - sX;
		float dy = y - sY;

		sX = x;
		sY = y;

		if (mIsFocused)
		{
			mCam.Rotate(glm::vec3(-dy, -dx, 0.0f) * cSensitivity);
		}
	}

	Mesh mModel;
	Entity mModelEntity{};
	Camera mCam;
	Entity mCameraEntity{};

	ShaderLibrary mShaders;
	OglProgramPipeline mPrograms;
	OglFramebuffer mSceneBuffer;

	bool mIsFocused = false;
};

int main()
{
	glabs::GlfwLife::Params glfwParams;
	glfwParams.CoreProfile = true;
	glfwParams.Version = { 4, 6 };
	glfwParams.EnableDebugContext = true;

	glabs::GlfwLife glfw(std::move(glfwParams));
	{
		glabs::AppContainer()
			.Bind<TestApp>()
			.Configure()
			.Run();
	}
}

