#include "glabs/rendering/camera2.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace glabs
{
	glm::mat4 Camera2::LookAt(const glm::vec3& eyePosition) const
	{
		return glm::lookAt(eyePosition, eyePosition + mFront, mUp);
	}

	void Camera2::Rotate(const glm::vec3& rotation)
	{
		SetRotation(mRotation + rotation);
	}

	void Camera2::SetRotation(const glm::vec3& rotation)
	{
		mRotation = rotation;
		WrapRotation();
		UpdateVectors();
	}

	const glm::vec3& Camera2::GetRotation() const
	{
		return mRotation;
	}

	const glm::vec3& Camera2::GetFront() const
	{
		return mFront;
	}

	const glm::vec3& Camera2::GetUp() const
	{
		return mUp;
	}

	const glm::vec3& Camera2::GetRight() const
	{
		return mRight;
	}

	float Camera2::WrapAngle(float radians)
	{
		float x = glm::mod(radians + glm::pi<float>(), glm::two_pi<float>());
		if (x < 0.0f)
		{
			x += glm::two_pi<float>();
		}
		return x - glm::pi<float>();
	}

	void Camera2::UpdateVectors()
	{
		// mFront.x = glm::cos(mRotation.x) * glm::cos(mRotation.y);
		// mFront.y = glm::cos(mRotation.y);
		// mFront.z = glm::sin(mRotation.x) * glm::cos(mRotation.y);

		auto rotation = glm::rotate(glm::mat4(1.0f), mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = glm::rotate(rotation, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = glm::rotate(rotation, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

		mFront = glm::normalize(glm::vec3(rotation * glm::vec4(cWorldFront, 1.0f)));

		mRight = glm::normalize(glm::cross(mFront, cWorldUp));
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}

	void Camera2::WrapRotation()
	{
		mRotation = {
			WrapAngle(mRotation.x),
			WrapAngle(mRotation.y),
			WrapAngle(mRotation.z),
		};
	}
}

