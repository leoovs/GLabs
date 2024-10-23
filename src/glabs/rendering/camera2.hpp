#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class Camera2
	{
	public:
		glm::mat4 LookAt(const glm::vec3& eyePosition) const;

		void Rotate(const glm::vec3& rotation);
		void SetRotation(const glm::vec3& rotation);

		const glm::vec3& GetRotation() const;
		const glm::vec3& GetFront() const;
		const glm::vec3& GetUp() const;
		const glm::vec3& GetRight() const;

	private:
		static float WrapAngle(float radians);

		void UpdateVectors();
		void WrapRotation();

		glm::vec3 mRotation = glm::vec3(0.0f);
		glm::vec3 mFront = cWorldFront;
		glm::vec3 mUp = cWorldUp;
		glm::vec3 mRight = cWorldRight;

		static constexpr glm::vec3 cWorldFront{ 0.0f, 0.0f, -1.0f };
		static constexpr glm::vec3 cWorldUp{ 0.0f, 1.0f, 0.0f };
		static constexpr glm::vec3 cWorldRight{ 1.0f, 0.0f, 0.0f };
	};
}

