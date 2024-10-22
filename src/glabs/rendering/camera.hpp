#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class Camera
	{
	public:
		Camera() = default;

		Camera(const glm::vec3& position);

		glm::mat4 GetLookAt() const;
		const glm::vec3& GetPosition() const;

		void Move(const glm::vec3& movement);

	private:
		glm::vec3 mPosition;
	};
}

