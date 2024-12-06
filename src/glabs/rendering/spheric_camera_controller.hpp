#pragma once

#include "glabs/rendering/camera.hpp"

#include <glm/gtc/constants.hpp>

namespace glabs
{
	class SphericCameraController
	{
	public:
		SphericCameraController() = default;

		SphericCameraController(Camera* controlledCamera);

		void SetCamera(Camera* controlledCamera);
		Camera* GetCamera() const;

		void SetAzimuth(float azimuth);
		void SetZenith(float zenith);
		void SetZenithMax(float zenithClamp);

		void IncrementAzimuth(float deltaAzimuth);
		void IncrementZenith(float deltaZenith);

		void OnMouseMove(float x, float y);

	private:
		void WrapAngles();

		void UpdateCamera();

		Camera* mControlledCamera = nullptr;
		float mAzimuth = glm::radians(0.0f);
		float mZenith = glm::half_pi<float>();
		float mZenithClamp = glm::radians(1.0f);

		float mLastMouseX = INFINITY;
		float mLastMouseY = INFINITY;
	};
}

