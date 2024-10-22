#include "glabs/rendering/camera.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace glabs
{
	Camera::Camera(const glm::vec3& position)
		: mPosition(position)
	{}

	glm::mat4 Camera::GetRotationXYZ() const
	{
		glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		return rotX * rotY * rotZ;
	}

	glm::mat4 Camera::GetLookAt() const
	{
		glm::vec4 up(0.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4 viewDirection(0.0f, 0.0f, -1.0f, 0.0f);
		glm::mat4 rotXYZ = GetRotationXYZ();

		// TODO: maybe make `up` vector configurable.
		return glm::lookAt(
			mPosition,
			mPosition + glm::vec3(rotXYZ * viewDirection),
			glm::normalize(glm::vec3(up))
		);
	}

	const glm::vec3& Camera::GetPosition() const
	{
		return mPosition;
	}

	const glm::vec3& Camera::GetRotation() const
	{
		return mRotation;
	}

	void Camera::Move(const glm::vec3& movement)
	{
		mPosition += movement;
	}

	void Camera::RelMove(const glm::vec3& movement)
	{
		mPosition += glm::vec3(GetRotationXYZ() * glm::vec4(movement, 1.0f));
	}

	void Camera::Rotate(const glm::vec3& rotation)
	{
		mRotation += rotation;
	}
}

