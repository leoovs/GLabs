#pragma once

#include "glabs/app/basic_app.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/rendering/camera.hpp"
#include "glabs/rendering/mesh.hpp"
#include "glabs/lab5/entity.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class Lab5App : public BasicApp
	{
	private:
		void OnStart() override;
		void OnUpdate(float dt) override;
		void OnMouseMove(float x, float y) override;
		void OnMouseScroll(float xoffset, float yoffset) override;

		void ShowMenu();
		void HandleInput(float dt);
		void RotateCamera(float dx, float dy);
		void UpdateCamera();
		void Render();

		void LoadShaders();

		Camera mCamera;
		float mDx = 0.0f;
		float mDy = 0.0f;
		float mAzimuth = 0.0f;
		float mZenith = glm::half_pi<float>() / 2.0f;
		float mRadius = 1.0f;
		Entity mCart;
		OglProgramPipeline mCartShaders;
		ShaderLibrary mShaders;
	};
}

