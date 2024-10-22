#include "glabs/rendering/camera.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace glabs
{
	Camera::Camera(const glm::vec3& position)
		: mPosition(position)
	{}

	glm::mat4 Camera::GetLookAt() const
	{
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::vec3 viewDirection(0.0f, 0.0f, -1.0f);

		return glm::lookAt(mPosition, mPosition + viewDirection, up);
	}

	const glm::vec3& Camera::GetPosition() const
	{
		return mPosition;
	}

	void Camera::Move(const glm::vec3& movement)
	{
		mPosition += movement;
	}
}

