#include "glabs/graphics/glfw_life.hpp"
#include "glabs/app/app_container.hpp"
#include "glabs/app/basic_app.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"
#include "glabs/pch.hpp"
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

		glfwSetInputMode(GetWindow().GetNativeWindow(), GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
	}

	void OnWindowResize(int32_t width, int32_t height) override
	{
		Render();
	}

	void Render()
	{
		Window& window = GetWindow();
		float aspect = window.GetWidth() / float(window.GetHeight());

		glm::mat4 proj = glm::perspective(glm::radians(60.0f), aspect, 0.01f, 20.0f);
		glm::mat4 view = mCam.LookAt(mCameraEntity.Translation);
		glm::mat4 model = mModelEntity.CalculateMatrix();
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, window.GetWidth(), window.GetHeight());

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

		window.Present();
	}

	void OnUpdate(float dt) override
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

		if (glm::length(movement) > glm::epsilon<float>())
		{
			movement = glm::normalize(movement);
		}

		mCameraEntity.Translation += movement * speed;

		Render();
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

		mCam.Rotate(glm::vec3(-dy, -dx, 0.0f) * cSensitivity);

		const glm::vec3& rotation = mCam.GetRotation();
	}

	Mesh mModel;
	Entity mModelEntity{};
	Camera mCam;
	Entity mCameraEntity{};

	ShaderLibrary mShaders;
	OglProgramPipeline mPrograms;
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

