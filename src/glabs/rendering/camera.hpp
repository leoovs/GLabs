#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class Camera
	{
	public:
		Camera() = default;

		Camera(
			const glm::vec3& position,
			const glm::vec3& viewDirection = glm::vec3(0.0f, 0.0f, -1.0f)
		);

		glm::mat4 GetLookAt() const;

		void Move(const glm::vec3& movement);

	private:
		glm::vec3 mPosition;
		glm::vec3 mViewDirection;
	};
}

