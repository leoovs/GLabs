#pragma once

#include "glabs/application/lab_task.hpp"
#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class LabTask_DrawThirdShape : public LabTask
	{
	public:
		enum class DisplayStrategy
		{
			A,
			B,
			C,
			Count_,
		};

		static constexpr const char* GetDisplayStrategyString(DisplayStrategy strat)
		{
			switch (strat)
			{
			case DisplayStrategy::A:
				return "Лицевые грани отображаются только вершинами";
			case DisplayStrategy::B:
				return "Лицевые грани закрашены, а обратные - линии";
			case DisplayStrategy::C:
				return "Каркасное отображение";
			default:
				return "???";
			}
		}

		LabTask_DrawThirdShape();
		~LabTask_DrawThirdShape() override = default;

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
		void LoadShaders();

		OglBuffer mShapePositions;
		OglBuffer mShapeIndices;
		OglGeometryInput mShapeGeometry;
		OglProgramPipeline mPrograms;
		ShaderLibrary mShapeShaders;

		bool mIsColorInterpolationEnabled = true;
		bool mIsRenderedCounterClockWise = false;
		DisplayStrategy mDisplayStrategy = DisplayStrategy::B;
		int mDisplayStrategyPeek = 1;
	};
}
