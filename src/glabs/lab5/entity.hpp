#pragma once

#include "glabs/pch.hpp"
#include "glabs/rendering/mesh.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace glabs
{
	struct Entity
	{
		Mesh Model;
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		glm::mat4 CalculcateModelMatrix() const
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);

			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			rotate = glm::rotate(rotate, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			rotate = glm::rotate(rotate, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 translate = glm::translate(glm::mat4(1.0f), Translation);

			return translate * rotate * scale;
		}
	};
}

