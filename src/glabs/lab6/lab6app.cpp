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
		mRenderer = std::make_unique<ForwardRenderer>(ForwardRenderer::Params{ 5 });

		LoadModel();
		LoadShaders();

		mCamera.SetEyePosition({ 9.0f, 6.0f, 13.0f });
		mCamera.LookAt({ 0.0f, 0.0f, 0.0f });

		PlaceSpheres();
		PlaceLights();
		AssignMaterials();

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	void Lab6App::OnUpdate(float dt)
	{
		ShowMenu();
		HandleInput(dt);
		AnimateSpheres(dt);
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

		ImGui::Begin("Lights");
		glm::vec3 camPos = mCamera.GetEyePosition();
		ImGui::Text("Camera position: %.2f %.2f %.2f", camPos.x, camPos.y, camPos.z);

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

		ShowMenu_Lights();

		ImGui::End();

		ImGui::Begin("Models");
		ShowMenu_Model();
		ImGui::End();

		ImGui::Render();
	}

	void Lab6App::ShowMenu_Lights()
	{
		DirectionalLight dirLight = mRenderer->GetDirectionalLight();
		ImGui::Text("Directional light");
		ImGui::DragFloat3("Direction", glm::value_ptr(dirLight.Direction), 0.1f, -1.0f, 1.0f);
		ImGui::ColorEdit3("Ambient", glm::value_ptr(dirLight.Ambient));
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(dirLight.Diffuse));
		ImGui::ColorEdit3("Specular", glm::value_ptr(dirLight.Specular));
		ImGui::NewLine();
		mRenderer->SetDirectionalLight(dirLight);

		for (size_t iLight = 0; iLight < mRenderer->GetLightCount(); iLight++)
		{
			Light light = mRenderer->GetLight(iLight);

			ImGui::PushID(iLight);
			if (ImGui::TreeNode("Light", "Light #%zu", iLight))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(light.Position.get()), 0.1f, -10.0f, 10.0f);
				ImGui::NewLine();
				ImGui::DragFloat("Linear", &light.Linear.get(), 0.25f, 0.0f, 10.0f);
				ImGui::DragFloat("Quadratic", &light.Quadratic.get(), 0.5f, 0.0f, 20.0f);
				if (light.Kind.get() == static_cast<int>(LightKind::Spot))
				{
					ImGui::Text("Spot light options");
					ImGui::DragFloat3("Direction", glm::value_ptr(light.Direction.get()), 0.1f, -10.0f, 10.0f);
					ImGui::DragFloat("Cut off", &light.CutOff.get(), 0.01f, 0.5f, 1.0f);
					ImGui::DragFloat("Outer cut off", &light.OuterCutOff.get(), 0.01f, 0.5f, 1.0f);
				}

				ImGui::NewLine();
				ImGui::ColorEdit3("Ambient", glm::value_ptr(light.Ambient.get()));
				ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.Diffuse.get()));
				ImGui::ColorEdit3("Specular", glm::value_ptr(light.Specular.get()));

				ImGui::Combo(
					"Kind",
					&light.Kind.get(),
					[](void* userdata, int kind) -> const char*
					{
						return LightKindToString(LightKind(kind)).data();
					},
					nullptr,
					2
				);

				bool enabled = 1 == light.Cast.get();
				ImGui::Checkbox("Enabled", &enabled);
				light.Cast.get() = enabled;

				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}

	void Lab6App::ShowMenu_Model()
	{
		bool animateAll = mAnimateSpheres[0] && mAnimateSpheres[1];
		bool animeAllPressed = ImGui::Checkbox("Animate all", &animateAll);
		if (animeAllPressed)
		{
			PlaceSpheres();
		}

		mAnimateSpheres[0] = mAnimateSpheres[1] = animateAll;

		for (int iModel = 0; iModel < std::size(mSpheres); iModel++)
		{
			ImGui::PushID(iModel);

			if (ImGui::TreeNode("Sphere", "Sphere #%d", iModel))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(mSphereTranslations[iModel]), 0.1f, -10.0f, 10.0f);
				ImGui::DragFloat3("Scale", glm::value_ptr(mSphereScales[iModel]), 0.1f, -10.0f, 10.0f);

				ForwardRenderer::Material& material = mSphereMaterials[iModel];

				ImGui::NewLine();
				ImGui::Text("Material");
				ImGui::ColorEdit3("Ambient", glm::value_ptr(material.Ambient));
				ImGui::ColorEdit3("Diffuse", glm::value_ptr(material.Diffuse));
				ImGui::ColorEdit3("Specular", glm::value_ptr(material.Specular));
				ImGui::DragFloat("Shininess", &material.Shininess, 1.0f, 0.1f, 200.0f);

				ImGui::NewLine();

				bool pressed = ImGui::Checkbox("Animate", mAnimateSpheres + iModel);
				if (pressed)
				{
					PlaceSpheres();
				}

				ImGui::TreePop();
			}

			ImGui::PopID();
		}
	}

	void Lab6App::Render()
	{
		mRenderer->SetCamera(mCamera);
		mRenderer->SetProjection(mProjection);

		mRenderer->BeginMeshPass();
		{
			for (int iModel = 0; iModel < std::size(mSpheres); iModel++)
			{
				mRenderer->RenderMeshWithMaterial(
					mSpheres[iModel],
					mSphereMaterials[iModel],
					glm::scale(glm::translate(glm::mat4(1.0f), mSphereTranslations[iModel]), mSphereScales[iModel])
				);
			}
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
		if (IsKeyDown(GLFW_KEY_LEFT_ALT))
		{
			speed = 8.0f;
		}

		movement *= speed * dt;

		mCamera.Move(movement);
	}

	void Lab6App::AnimateSpheres(float dt)
	{
		float(*animationFunctions[cMaxModels])(float)
		{
			[](float theta){ return 3 * glm::cos(5 * theta) + 6.0f; },
			[](float theta){ return 14 + 4 * glm::sin(14 * theta); },
		};

		for (int iModel = 0; iModel < std::size(mSphereThetas); iModel++)
		{
			if (!mAnimateSpheres[iModel])
			{
				continue;
			}

			float& theta = mSphereThetas[iModel];
			float animationSpeed = mAnimationSpeed[iModel];

			theta += dt * animationSpeed;
			float radius = animationFunctions[iModel](theta);

			float x = radius * glm::cos(theta);
			float z = radius * glm::sin(theta);

			mSphereTranslations[iModel].x = x;
			mSphereTranslations[iModel].z = z;
		}
	}

	void Lab6App::PlaceSpheres()
	{
		mSphereScales[0] = mSphereScales[1] = glm::vec3(2.9f);
		mSphereTranslations[0] = glm::vec3(-2.9f, 0.0f, 0.0f);
		mSphereTranslations[1] = glm::vec3(-2.9f + 4 * 2.9f, 0.0f, 0.0f);
		mSphereThetas[0] = mSphereThetas[1] = 0.0f;
	}

	void Lab6App::PlaceLights()
	{
		Light first = mRenderer->GetLight(0);
		first.Kind.get() = static_cast<int>(LightKind::Spot);
		first.Position.get() = glm::vec3(6.0f, 3.0f, 3.0f);
		first.Direction.get() = glm::vec3(2.0f, -1.0f, -1.0f);
		first.Diffuse.get() = glm::vec3(1.0f);
		first.Specular.get() = glm::vec3(1.0f);
		first.Linear.get() = 0.35f;
		first.Quadratic.get() = 0.44f;
		first.Cast.get() = 1;

		Light second = mRenderer->GetLight(1);
		second.Kind.get() = static_cast<int>(LightKind::Spot);
		second.Position.get() = glm::vec3(-1.0f, 3.0f, 3.0f);
		second.Direction.get() = glm::vec3(-1.0f, -1.0f, -1.0f);
		second.Diffuse.get() = glm::vec3(1.0f);
		second.Specular.get() = glm::vec3(1.0f);
		second.Linear.get() = 0.35f;
		second.Quadratic.get() = 0.44f;
		second.Cast.get() = 1;

		Light third = mRenderer->GetLight(2);
		third.Kind.get() = static_cast<int>(LightKind::Point);
		third.Position.get() = glm::vec3(-4.5f, 3.5f, 8.0f);
		third.Diffuse.get() = glm::vec3(1.0f, 0.0f, 0.0f);
		third.Specular.get() = glm::vec3(1.0f, 0.5f, 0.5f);
		third.Linear.get() = 0.35f;
		third.Quadratic.get() = 0.44f;
		third.Cast.get() = 1;

		Light fourth = mRenderer->GetLight(3);
		fourth.Kind.get() = static_cast<int>(LightKind::Point);
		fourth.Position.get() = glm::vec3(2.3f, 3.5f, 10.0f);
		fourth.Diffuse.get() = glm::vec3(0.0f, 0.0f, 1.0f);
		fourth.Specular.get() = glm::vec3(0.5f, 0.5f, 1.0f);
		fourth.Linear.get() = 0.35f;
		fourth.Quadratic.get() = 0.44f;
		fourth.Cast.get() = 1;

		Light fifth = mRenderer->GetLight(4);
		fifth.Kind.get() = static_cast<int>(LightKind::Point);
		fifth.Position.get() = glm::vec3(-7.0f, 3.5f, -5.5f);
		//fifth.Diffuse.get() = glm::vec3(0.0f, 1.0f, 0.0f);
		//fifth.Specular.get() = glm::vec3(1.0f, 0.0f, 1.0f);
		fifth.Diffuse.get() = glm::vec3(1.0f, 1.0f, 1.0f);
		fifth.Specular.get() = glm::vec3(1.0f, 1.0f, 1.0f);
		fifth.Linear.get() = 0.35f;
		fifth.Quadratic.get() = 0.44f;
		fifth.Cast.get() = 1;
	}

	void Lab6App::AssignMaterials()
	{
		// Pearl.
		ForwardRenderer::Material& first = mSphereMaterials[0];
		first.Ambient = glm::vec3(0.25f, 0.21f, 0.21f);
		first.Diffuse = glm::vec3(1.0f, 0.82f, 0.82f);
		first.Specular = glm::vec3(0.296f, 0.296f, 0.296f);
		first.Shininess = 11.26f;

		// Green plastic.
		ForwardRenderer::Material& second = mSphereMaterials[1];
		second.Ambient = glm::vec3(0.1f);
		second.Diffuse = glm::vec3(0.1f, 0.35f, 0.1f);
		second.Specular = glm::vec3(0.45f, 0.55f, 0.45f);
		second.Shininess = 1.0f;
	}

	void Lab6App::LoadModel()
	{
		mSpheres[0] = ObjImporter()
			.OpenFile("models/unit_sphere.obj")
			.LoadAllShapes()
			.Build();

		mSpheres[1] = ObjImporter()
			.OpenFile("models/unit_sphere.obj")
			.LoadAllShapes()
			.Build();

		mSphereTranslations[0] = glm::vec3(0.0f);
		mSphereTranslations[1] = glm::vec3(3.0f, 0.0f, 1.0f);

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

