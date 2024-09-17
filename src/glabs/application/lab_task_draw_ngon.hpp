#include "glabs/application/lab_task.hpp"
#include "glabs/application/shape_math.hpp"
#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	class Lab2Application;

	class LabTask_DrawNGon : public LabTask
	{
	public:
		enum class DrawStrategy
		{
			Points,
			Lines,
		};

		static constexpr const char* GetDrawStrategyString(DrawStrategy strat)
		{
			switch (strat)
			{
			case DrawStrategy::Points:
				return "Точки";
			case DrawStrategy::Lines:
				return "Линии";
			default:
				return "None";
			}
		};

		LabTask_DrawNGon(Lab2Application& app);

		~LabTask_DrawNGon() override = default;

		LabTaskKind GetKind() const override;
		LabTaskKind GetNext() const override;
		LabTaskKind GetPrev() const override;

		void OnEnter() override;
		void OnQuit() override;

		void OnMainMenu() override;
		void OnUpdate() override;
		void OnRender() override;

	private:
		void RenderPoints();
		void RenderLines();

		void LoadShaders();

		void RegenerateNGon();

		void LoadNGon();

		Lab2Application* mApp = nullptr;

		static constexpr int cNGonMinSize = 3;
		static constexpr int cNGonMaxSize = 10;

		int mNGonSize = cNGonMinSize;
		float mNGonRadius = 0.5f;
		Vector2f mNGonCenter = Vector2f(0.0f, 0.0f);
		std::vector<Vector2f> mNGon;

		int mDrawStrategyUIPeek = 0;
		DrawStrategy mDrawStrategy = DrawStrategy::Points;

		float mPointSize = 1.0f;
		bool mSmoothPoint = false;
		float mLineWidth = 1.0f;
		bool mSmoothLine = false;

		OglBuffer mNGonPositions;
		OglGeometryInput mNGonGeometry;
		OglProgramPipeline mShaders;
		ShaderLibrary mNGonShaders;
	};
}

