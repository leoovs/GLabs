#include "glabs/lab5/lab5app.hpp"

#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/rendering/obj_importer.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glabs
{
	void Lab5App::OnStart()
	{
		mCart.Model = ObjImporter()
			.OpenFile("models/stormtrooper.obj")
			.LoadAllShapes()
			.Build();

		mCart.Translation = { 0.0f, 0.0f, 0.0f };
		mCamera.SetEyePosition(glm::vec3(0.0f, 0.0f, 10.0f));
		mCamera.LookAt({ 0.0f, 0.0f, 0.0f });

		LoadShaders();

		glEnable(GL_DEPTH_TEST);
	}

	void Lab5App::OnUpdate(float dt)
	{
		ShowMenu();
		Render();
	}

	void Lab5App::OnMouseMove(float x, float y)
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			return;
		}

		static float sX = x;
		static float sY = y;

		float dx = sX - x;
		float dy = sY - y;

		sX = x;
		sY = y;

		RotateCamera(dx, dy);
	}

	void Lab5App::ShowMenu()
	{
		GetImGui().NewFrame();

		ImGui::Begin("A Window");
		ImGui::Text("Elevation: %.3f (%.3f)", mElevation, glm::degrees(mElevation));
		ImGui::DragFloat3("Position", glm::value_ptr(mCart.Translation), 0.5f, -10.0f, 10.0f);
		ImGui::End();

		ImGui::Render();
	}

	void Lab5App::HandleInput(float dt)
	{
		glm::vec3 movement = glm::vec3(0.0f);
		float speed = 5.0f;

		glm::vec3 front = mCamera.GetFront();
		front.y = 0.0f;
		front = glm::normalize(front);

		glm::vec3 right = mCamera.GetRight();
		right.y = 0.0f;
		right = glm::normalize(right);

		if (IsKeyDown(GLFW_KEY_D))
		{
			movement += right;
		}
		if (IsKeyDown(GLFW_KEY_A))
		{
			movement -= right;
		}
		if (IsKeyDown(GLFW_KEY_W))
		{
			movement += front;
		}
		if (IsKeyDown(GLFW_KEY_S))
		{
			movement -= front;
		}
		if (IsKeyDown(GLFW_KEY_SPACE))
		{
			movement.y += 1.0f;
		}
		if (IsKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			movement.y -= 1.0f;
		}

		movement = glm::length(movement) < glm::epsilon<float>()
			? movement
			: glm::normalize(movement);

		mCamera.Move(speed * movement * dt);

		mCamera.LookAt(mCart.Translation + glm::vec3(0.0f, 0.5f, 0.0f));
	}

	void Lab5App::RotateCamera(float dx, float dy)
	{
		if (GLFW_RELEASE == glfwGetMouseButton(GetWindow().GetNativeWindow(), GLFW_MOUSE_BUTTON_LEFT))
		{
			return;
		}

		float radius = 5.0f;
		float speed = 0.01f;

		mAzimuth -= dx * speed;
		mElevation += dy * speed;

		mElevation = std::clamp(mElevation, 0.1f, glm::pi<float>() - 0.1f);

		glm::vec3 position
		{
			radius * glm::sin(mElevation) * glm::cos(mAzimuth),
			radius * glm::cos(mElevation),
			radius * glm::sin(mElevation) * glm::sin(mAzimuth)
		};

		mCamera.SetEyePosition(position);
		mCamera.LookAt({ 0.0f, 0.0f, 0.0f });
	}

	void Lab5App::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Window& window = GetWindow();
		DearImGui& imgui = GetImGui();

		float aspect = window.GetWidth() / float(window.GetHeight());

		glm::mat4 model = mCart.CalculcateModelMatrix();
		glm::mat4 view = mCamera.CalculateLookAt();
		glm::mat4 projection = glm::perspective(45.0f, aspect, 0.01f, 100.0f);

		mCartShaders[ShaderStage::Vertex].Get().SetUniform("uInvTranspM", glm::transpose(glm::inverse(model)));
		mCartShaders[ShaderStage::Vertex].Get().SetUniform("uMVP", projection * view * model);
		mCartShaders[ShaderStage::Vertex].Get().SetUniform("uCameraFront", mCamera.GetFront());

		mCartShaders.BindToPipeline();
		mCart.Model.ForEachShape([](const Submesh& shape)
			{
				shape.GetGeometry().BindToPipeline();
				glDrawArrays(GL_TRIANGLES, 0, shape.GetVertexCount());
			}
		);

		imgui.RenderDrawData(ImGui::GetDrawData());
		window.Present();
	}

	void Lab5App::LoadShaders()
	{
		mCartShaders = OglProgramPipeline({ "Cart shaders" });

		mCartShaders.SetProgram(mShaders[ShaderStage::Vertex].FetchFromFile("glsl/cart_vert.glsl", "cart"));
		mCartShaders.SetProgram(mShaders[ShaderStage::Fragment].FetchFromFile("glsl/cart_frag.glsl", "cart"));
	}
}

