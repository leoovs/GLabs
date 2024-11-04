#include "glabs/rendering/camera.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace glabs
{
	glm::mat4 Camera::CalculateLookAt() const
	{
		return glm::lookAt(mEyePosition, mEyePosition + mFront, mUp);
	}

	void Camera::Rotate(const glm::vec3& rotation)
	{
		SetRotation(mRotation + rotation);
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{
		mRotation = rotation;
		WrapRotation();
		UpdateVectors();
	}

	void Camera::LookAt(const glm::vec3& eyeTarget)
	{
		glm::vec3 direction = mEyePosition - eyeTarget;

		direction = glm::normalize(direction);

		mRotation.y = std::atan2(direction.x, direction.z);
		mRotation.x = -std::asin(direction.y);

		WrapRotation();
		UpdateVectors();
	}

	void Camera::Move(const glm::vec3& movement)
	{
		SetEyePosition(mEyePosition + movement);
	}

	void Camera::SetEyePosition(const glm::vec3& position)
	{
		mEyePosition = position;
	}

	const glm::vec3& Camera::GetRotation() const
	{
		return mRotation;
	}

	const glm::vec3& Camera::GetEyePosition() const
	{
		return mEyePosition;
	}

	const glm::vec3& Camera::GetFront() const
	{
		return mFront;
	}

	const glm::vec3& Camera::GetUp() const
	{
		return mUp;
	}

	const glm::vec3& Camera::GetRight() const
	{
		return mRight;
	}

	float Camera::WrapAngle(float radians)
	{
		float x = glm::mod(radians + glm::pi<float>(), glm::two_pi<float>());
		if (x < 0.0f)
		{
			x += glm::two_pi<float>();
		}
		return x - glm::pi<float>();
	}

	void Camera::UpdateVectors()
	{
		auto rotation = glm::rotate(glm::mat4(1.0f), mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = glm::rotate(rotation, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::rotate(rotation, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

		mFront = glm::normalize(glm::vec3(rotation * glm::vec4(cWorldFront, 1.0f)));

		mRight = glm::normalize(glm::cross(mFront, cWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}

	void Camera::WrapRotation()
	{
		mRotation = {
			WrapAngle(mRotation.x),
			WrapAngle(mRotation.y),
			WrapAngle(mRotation.z),
		};
	}
}

