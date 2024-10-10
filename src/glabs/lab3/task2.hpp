#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/lab3/task.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class Task2 : public Task
	{
	public:
		Task2();
		~Task2() override = default;

		TaskName GetName() const override;
		TaskName GetNext() const override;
		TaskName GetPrev() const override;

		std::string_view GetTitle() const override;

		void OnMainMenu() override;
		void OnRender() override;

	private:
		void LoadShapes();
		void LoadShaders();

		OglBuffer mShapesPositions;
		OglBuffer mShapesIndices;
		OglGeometryInput mShapesGeometryInput;

		ShaderLibrary mShaders;
		OglProgramPipeline mPrograms;

		float mZoom = 1.0f;

		glm::mat4 mLineModel = glm::mat4(1.0f);
		glm::mat4 mTriangleModel = glm::mat4(1.0f);
		glm::mat4 mRectangleModel = glm::mat4(1.0f);

		bool mShowLine = true;
		bool mShowTriangle = true;
		bool mShowRect = true;

		bool mTransformLine = false;
		bool mTransformTriangle = false;
		bool mTransformRect = false;
	};
}

