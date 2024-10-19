#include "glabs/rendering/camera.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace glabs
{
	Camera::Camera(const glm::vec3& position, const glm::vec3& viewDirection)
		: mPosition(position)
		, mViewDirection(glm::normalize(viewDirection))
	{}

	glm::mat4 Camera::GetLookAt() const
	{
		return glm::lookAt(mPosition, mPosition + mViewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void Camera::Move(const glm::vec3& movement)
	{
		mPosition += movement;
	}
}

