#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/pch.hpp"
#include "glabs/lab4/task.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class Task3 : public Task
	{
	public:
		enum class Projection
		{
			Ortho,
			Perspective,

			Count_
		};

		static constexpr std::string_view ProjectionToStr(Projection proj)
		{
			switch (proj)
			{
			case Projection::Ortho:
				return "Параллельная";
			case Projection::Perspective:
				return "Перспективная";
			}

			return "???";
		}

		Task3();
		~Task3() override = default;

		void OnUI() override;
		void OnRender() override;

		TaskName GetName() const override;
		TaskName GetNext() const override;
		TaskName GetPrev() const override;

	private:
		void SetupProjections();
		void SetupCube();
		void LoadShaders();

		Projection mProjection = Projection::Ortho;
		bool mCubeWireframe = false;

		glm::mat4 mOrthoProjection = glm::mat4(1.0f);
		glm::mat4 mPerspectiveProjection = glm::mat4(1.0f);
		glm::mat4* mCurrentProjection = &mOrthoProjection;

		glm::mat4 mModel = glm::mat4(1.0f);
		glm::vec3 mCubeTranslation = glm::vec3(0.0f);
		glm::vec3 mCubeRotation = glm::vec3(0.0f);

		OglBuffer mCubeVertexBuffer;
		OglBuffer mCubeIndexBuffer;
		OglGeometryInput mCubeGeometryInput;
		OglProgramPipeline mCubePrograms;

		ShaderLibrary mShaders;
	};
}

