#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class Camera
	{
	public:
		Camera() = default;

		Camera(const glm::vec3& position);

		glm::mat4 GetRotationXYZ() const;
		glm::mat4 GetLookAt() const;
		const glm::vec3& GetPosition() const;
		const glm::vec3& GetRotation() const;

		void Move(const glm::vec3& movement);
		void RelMove(const glm::vec3& movement);
		void Rotate(const glm::vec3& rotation);

	private:
		glm::vec3 mPosition;
		glm::vec3 mRotation;
	};
}

