#pragma once

#include "glabs/app/basic_app.hpp"
#include "glabs/lab6/forward_renderer.hpp"
#include "glabs/rendering/camera.hpp"
#include "glabs/rendering/shader_library.hpp"
#include <glm/gtc/constants.hpp>

namespace glabs
{
	class Lab6App : public BasicApp
	{
	private:
		std::string_view GetName() const override;

		void OnStart() override;
		void OnUpdate(float dt) override;
		void OnMouseMove(float x, float y) override;
		void OnWindowResize(int32_t width, int32_t height) override;

		void ShowMenu();
		void Render();
		void HandleInput(float dt);

		void LoadModel();
		void LoadShaders();

		Mesh mStormtrooper;
		Mesh mSphere;
		glm::mat4 mStormtrooperTranslation;
		glm::mat4 mSphereTranslation;

		glm::mat4 mProjection;
		Camera mCamera;
		std::unique_ptr<ForwardRenderer> mRenderer;
		ShaderLibrary mShaders;

		float mAzimuth = glm::half_pi<float>();
		float mZenith = -glm::half_pi<float>();
		float mRadius = 1.0f;

		float mLastX = 0.0f;
		float mLastY = 0.0f;
		bool mFirstFocus = true;
		bool mCaptureScene = false;
	};
}

