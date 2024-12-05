#include "glabs/rendering/first_person_camera_controller.hpp"

namespace glabs
{
	SphericCameraController::SphericCameraController(Camera* controlledCamera)
		: mControlledCamera(controlledCamera)
	{}

	void SphericCameraController::SetCamera(Camera* controlledCamera)
	{
		mControlledCamera = controlledCamera;
		UpdateCamera();
	}

	Camera* SphericCameraController::GetCamera() const
	{
		return mControlledCamera;
	}

	void SphericCameraController::SetAzimuth(float azimuth)
	{
		mAzimuth = azimuth;
		WrapAngles();
		UpdateCamera();
	}

	void SphericCameraController::SetZenith(float zenith)
	{
		mZenith = zenith;
		WrapAngles();
		UpdateCamera();
	}

	void SphericCameraController::SetZenithMax(float zenithClamp)
	{
		mZenithClamp = zenithClamp;
		WrapAngles();
		UpdateCamera();
	}

	void SphericCameraController::IncrementAzimuth(float deltaAzimuth)
	{
		SetAzimuth(mAzimuth + deltaAzimuth);
	}

	void SphericCameraController::IncrementZenith(float deltaZenith)
	{
		SetZenith(mZenith + deltaZenith);
	}

	void SphericCameraController::OnMouseMove(float x, float y)
	{
		if (INFINITY == mLastMouseX || INFINITY == mLastMouseY)
		{
			mLastMouseX = x;
			mLastMouseY = y;
		}

		float dx = x - mLastMouseX;
		float dy = y - mLastMouseY;

		float speed = 0.01f;
		IncrementZenith(dy * speed);
		IncrementAzimuth(dx * speed);

		std::cout << glm::degrees(mZenith) << ' ' << glm::degrees(mAzimuth) << '\n';

		mLastMouseX = x;
		mLastMouseY = y;
	}

	void SphericCameraController::WrapAngles()
	{
		float lowerClamp = mZenithClamp;
		float upperClamp = glm::pi<float>() - lowerClamp;

		mZenith = std::clamp(mZenith, lowerClamp, upperClamp);

		if (mAzimuth < 0.0f)
		{
			mAzimuth = glm::two_pi<float>();
		}
		else if (mAzimuth > glm::two_pi<float>())
		{
			mAzimuth = 0.0f;
		}
	}

	void SphericCameraController::UpdateCamera()
	{
		if (!mControlledCamera)
		{
			return;
		}

		glm::vec3 direction;
		direction.x = glm::sin(mZenith) * glm::cos(mAzimuth);
		direction.y = glm::cos(mZenith);
		direction.z = glm::sin(mZenith) * glm::sin(mAzimuth);

		mControlledCamera->LookAt(mControlledCamera->GetEyePosition() + direction);
	}
}

