#include "glabs/lab6/lab6app.hpp"
#include "glabs/lab6/forward_renderer.hpp"
#include "glabs/rendering/obj_importer.hpp"
#include "imgui_internal.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glabs
{
	std::string_view Lab6App::GetName() const
	{
		return "lab6";
	}

	void Lab6App::OnStart()
	{
		mRenderer = std::make_unique<ForwardRenderer>(ForwardRenderer::Params{ 8 });

		LoadModel();
		LoadShaders();

		DirectionalLight dirLight;
		dirLight.Direction = { 0.0f, -1.0f, 0.0f };
		dirLight.Ambient = glm::vec3(0.0f);
		dirLight.Diffuse = glm::vec3(1.0f);
		dirLight.Specular = glm::vec3(1.0f);

		mRenderer->SetDirectionalLight(dirLight);
		mRenderer->GetPointLight(0).Position.get() = { 0.0f, 2.0f, -3.0f };
		mRenderer->GetPointLight(0).Cast.get() = 1;
		mRenderer->GetPointLight(0).Ambient.get() = { 1.0f, 1.0f, 1.0f };
		mRenderer->GetPointLight(0).Specular.get() = { 1.0f, 1.0f, 1.0f };
		mRenderer->GetPointLight(0).Linear.get() = 0.7f;
		mRenderer->GetPointLight(0).Quadratic.get() = 1.8f;

		mCamera.SetEyePosition({ 0.0f, 5.0f, 0.0f });
		mCamera.LookAt({ 0.0f, 0.0f, -1.0f });
	}

	void Lab6App::OnUpdate(float dt)
	{
		ShowMenu();
		HandleInput(dt);
		Render();
	}

	void Lab6App::OnMouseMove(float x, float y)
	{
		if (!mCaptureScene)
		{
			return;
		}

		if (mFirstFocus)
		{
			mFirstFocus = !mFirstFocus;
			mLastX = x;
			mLastY = y;
		}

		float speed = 0.01f;
		float dx = mLastX - x;
		float dy = mLastY - y;

		mZenith += dy * speed;
		mAzimuth -= dx * speed;

		mZenith = std::clamp(mZenith, -glm::pi<float>(), 0.0f);

		mLastX = x;
		mLastY = y;

		glm::vec3 direction
		{
			glm::sin(mZenith) * glm::cos(mAzimuth),
			glm::cos(mZenith),
			glm::sin(mZenith) * glm::sin(mAzimuth),
		};

		mCamera.LookAt(mCamera.GetEyePosition() + direction);
	}

	void Lab6App::OnWindowResize(int32_t width, int32_t height)
	{
		mProjection = glm::perspective(glm::radians(45.0f), width / float(height), 0.01f, 1000.0f);
		glViewport(0, 0, width, height);
	}

	void Lab6App::ShowMenu()
	{
		GetImGui().NewFrame();

		ImGui::Begin("Scene");
		if (ImGui::Checkbox("Focused", &mCaptureScene))
		{
			if (mCaptureScene)
			{
				glfwSetInputMode(GetWindow().GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			ImGui::SetWindowCollapsed(ImGui::GetCurrentWindow(), true);
		}
		ImGui::Separator();
		ImGui::NewLine();

		DirectionalLight dirLight = mRenderer->GetDirectionalLight();
		ImGui::Text("Directional light");
		ImGui::DragFloat3("Direction", glm::value_ptr(dirLight.Direction));
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(dirLight.Diffuse));
		ImGui::NewLine();
		mRenderer->SetDirectionalLight(dirLight);

		for (size_t iLight = 0; iLight < mRenderer->GetPointLightCount(); iLight++)
		{
			PointLight light = mRenderer->GetPointLight(iLight);

			ImGui::PushID(iLight);
			if (ImGui::TreeNode("Light", "Light #%zu", iLight))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(light.Position.get()), 0.1f, -10.0f, 10.0f);
				ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.Diffuse.get()));

				bool enabled = 1 == light.Cast.get();
				ImGui::Checkbox("Enabled", &enabled);
				light.Cast.get() = enabled;

				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		ImGui::End();

		ImGui::Render();
	}

	void Lab6App::Render()
	{
		mRenderer->SetCamera(mCamera);
		mRenderer->SetProjection(mProjection);

		mRenderer->BeginMeshPass();
		{
			ForwardRenderer::Material m;
			m.Shininess = 30.0f;

			mRenderer->RenderMeshWithMaterial(mStormtrooper, m, mStormtrooperTranslation);
			mRenderer->RenderMeshWithMaterial(mSphere, m, mSphereTranslation);
		}
		mRenderer->EndMeshPass();

		mRenderer->BeginLightBoxPass();
		{
			mRenderer->RenderLightBoxes();
		}
		mRenderer->EndLightBoxPass();

		GetImGui().RenderDrawData(ImGui::GetDrawData());

		GetWindow().Present();
	}

	void Lab6App::HandleInput(float dt)
	{
		if (IsKeyDown(GLFW_KEY_ESCAPE))
		{
			mCaptureScene = false;
			mFirstFocus = true;
			glfwSetInputMode(GetWindow().GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (!mCaptureScene)
		{
			return;
		}

		glm::vec3 movement = glm::vec3(0.0f);

		glm::vec3 horizontalFront = mCamera.GetFront();
		horizontalFront.y = 0.0f;
		horizontalFront = glm::normalize(horizontalFront);

		glm::vec3 horizontalRight = mCamera.GetRight();
		horizontalRight.y = 0.0f;
		horizontalRight = glm::normalize(horizontalRight);

		if (IsKeyDown(GLFW_KEY_W))
		{
			movement += horizontalFront;
		}
		if (IsKeyDown(GLFW_KEY_A))
		{
			movement -= horizontalRight;
		}
		if (IsKeyDown(GLFW_KEY_S))
		{
			movement -= horizontalFront;
		}
		if (IsKeyDown(GLFW_KEY_D))
		{
			movement += horizontalRight;
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

		float speed = 2.0f;
		movement *= speed * dt;

		mCamera.Move(movement);
	}

	void Lab6App::LoadModel()
	{
		mStormtrooper = ObjImporter()
			.OpenFile("models/stormtrooper.obj")
			.LoadAllShapes()
			.Build();

		mSphere = ObjImporter()
			.OpenFile("models/sphere.obj")
			.LoadAllShapes()
			.Build();

		mStormtrooperTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
		mSphereTranslation = glm::scale(glm::mat4(1.0f), glm::vec3(1/8.0f));
		mSphereTranslation = glm::translate(mSphereTranslation, glm::vec3(1.0f, 0.5f, -3.0f));

		mProjection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.01f, 1000.0f);
	}

	void Lab6App::LoadShaders()
	{
		OglProgramPipeline& meshPass = mRenderer->GetPipeline(ForwardRenderer::ShaderPipeline::MeshPass);
		meshPass.SetProgram(mShaders[ShaderStage::Vertex].FetchFromFile("glsl/fwd_vert.glsl", "mesh"));
		meshPass.SetProgram(mShaders[ShaderStage::Fragment].FetchFromFile("glsl/fwd_frag.glsl", "mesh"));

		OglProgramPipeline& lightBoxPass = mRenderer->GetPipeline(ForwardRenderer::ShaderPipeline::LightBoxPass);
		lightBoxPass.SetProgram(mShaders[ShaderStage::Vertex].FetchFromFile("glsl/fwd_lightbox_vert.glsl", "lightbox"));
		lightBoxPass.SetProgram(mShaders[ShaderStage::Fragment].FetchFromFile("glsl/fwd_lightbox_frag.glsl", "lightbox"));
	}
}

