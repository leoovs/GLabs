#include "glabs/graphics/glfw_life.hpp"
#include "glabs/app/app_container.hpp"
#include "glabs/app/basic_app.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/pch.hpp"
#include "glabs/rendering/mesh.hpp"
#include "glabs/rendering/shader_library.hpp"
#include "glabs/rendering/camera.hpp"

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
	void OnStart() override
	{
		mModel = Mesh::FromObjFile("glsl/sphere.obj");

		mPrograms = OglProgramPipeline({ "shaders" });

		mPrograms.SetProgram(mShaders[ShaderStage::Vertex]
			.FetchFromFile("glsl/vert.glsl", "basic"));
		mPrograms.SetProgram(mShaders[ShaderStage::Fragment]
			.FetchFromFile("glsl/frag.glsl", "basic"));

		mCam = Camera(glm::vec3(0.0f, 0.0f, 13.0f));
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
		glm::mat4 view = mCam.GetLookAt();

		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, window.GetWidth(), window.GetHeight());

		mPrograms.BindToPipeline();

		mPrograms[ShaderStage::Vertex].Get().SetUniform("uMVP", proj * view * mEntity.CalculateMatrix());

		mModel.BindToPipeline();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// glDrawArrays(GL_TRIANGLES, 0, mModel.GetVertexCount());
		glDrawElements(GL_TRIANGLES, mModel.GetIndexCount(), GL_UNSIGNED_INT, nullptr);

		window.Present();
	}

	void OnUpdate(float dt) override
	{
		float speed = 2.0f;
		if (IsKeyDown(GLFW_KEY_D))
		{
			glm::vec3 right(speed, 0.0f, 0.0f);
			mCam.Move(right * dt);
		}
		if (IsKeyDown(GLFW_KEY_A))
		{
			glm::vec3 left(-speed, 0.0f, 0.0f);
			mCam.Move(left * dt);
		}
		if (IsKeyDown(GLFW_KEY_W))
		{
			glm::vec3 forward(0.0f, 0.0f, -speed);
			mCam.Move(forward * dt);
		}
		if (IsKeyDown(GLFW_KEY_S))
		{
			glm::vec3 forward(0.0f, 0.0f, speed);
			mCam.Move(forward * dt);
		}

		Render();
	}

	Mesh mModel;
	Entity mEntity;
	Camera mCam;

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

