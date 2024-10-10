#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/lab3/task.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class Task3 : public Task
	{
	public:
		Task3();
		~Task3() override = default;

		TaskName GetName() const override;
		TaskName GetNext() const override;
		TaskName GetPrev() const override;

		std::string_view GetTitle() const override;

		void OnMainMenu() override;
		void OnRender() override;

	private:
		void LoadShape();
		void LoadShaders();

		OglBuffer mPositions;
		OglBuffer mIndices;
		OglGeometryInput mGeometry;
		ShaderLibrary mShaders;
		OglProgramPipeline mPrograms;

		float mZoom = 1.0f;
		float mRotation = 1.0f;
		glm::vec3 mTranslation = glm::vec3(0.0f);
		glm::vec3 mScale = glm::vec3(1.0f);
	};
}

