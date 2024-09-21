#pragma once

#include "glabs/application/lab_task.hpp"
#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class LabTask_DrawSecondShapeTriangles : public LabTask
	{
	public:
		enum class DrawStrategy
		{
			Triangles,
			TriangleStrip,
			TriangleFan,
			Count_,
		};

		static constexpr const char* GetDrawStrategyString(DrawStrategy strat)
		{
			switch (strat)
			{
			case DrawStrategy::Triangles:
				return "Треугольник";
			case DrawStrategy::TriangleStrip:
				return "Лента треугольников";
			case DrawStrategy::TriangleFan:
				return "Веер треугольников";
			default:
				return "???";
			}
		}

		LabTask_DrawSecondShapeTriangles();
		~LabTask_DrawSecondShapeTriangles() override = default;

		LabTaskKind GetKind() const override;
		LabTaskKind GetNext() const override;
		LabTaskKind GetPrev() const override;

		void OnEnter() override;
		void OnQuit() override;

		void OnMainMenu() override;
		void OnUpdate() override;
		void OnRender() override;

	private:
		void LoadShape();
		void LoadShapeIndicies();
		void LoadShaders();

		OglBuffer mShapePositions;
		OglBuffer mShapeIndicies;
		OglGeometryInput mShapeGeometry;
		OglProgramPipeline mPrograms;
		ShaderLibrary mShapeShaders;

		int mDrawStrategyPeek = 0;
		DrawStrategy mDrawStrategy = DrawStrategy::Triangles;
		bool mIsColorInterpolationEnabled = false;
	};
}

