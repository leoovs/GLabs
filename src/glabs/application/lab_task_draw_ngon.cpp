#include "glabs/application/lab_task_draw_ngon.hpp"

namespace glabs
{
	LabTask_DrawNGon::LabTask_DrawNGon(Lab2Application& app)
		: mApp(&app)
	{
		RegenerateNGon();
		LoadShaders();
	}

	LabTaskKind LabTask_DrawNGon::GetKind() const
	{
		return LabTaskKind::DrawNGon;
	}

	void LabTask_DrawNGon::OnEnter()
	{
	}

	void LabTask_DrawNGon::OnQuit()
	{
	}

	void LabTask_DrawNGon::OnMainMenu()
	{
		if (ImGui::SliderInt("N", &mNGonSize, cNGonMinSize, cNGonMaxSize))
		{
			RegenerateNGon();
		}

		bool drawStrategyChaned = ImGui::ListBox(
			"Способ рисования",
			&mDrawStrategyUIPeek,
			[](void* userData, int item) -> const char*
			{
				return GetDrawStrategyString(static_cast<DrawStrategy>(item));
			},
			nullptr,
			2
		);
		mDrawStrategy = static_cast<DrawStrategy>(mDrawStrategyUIPeek);

		if (drawStrategyChaned)
		{
			RegenerateNGon();
		}

		ImGui::Text("%zu", mNGon.size());

		switch (mDrawStrategy)
		{
		case DrawStrategy::Points:
			ImGui::SliderFloat("Размер точки", &mPointSize, 1.0f, 100.0f);
			ImGui::Checkbox("Сглаживание точек", &mSmoothPoint);
			break;

		case DrawStrategy::Lines:
			ImGui::SliderFloat("Толщина линии", &mLineWidth, 1.0f, 100.0f);
			break;
		}
	}

	void LabTask_DrawNGon::OnUpdate()
	{
	}

	void LabTask_DrawNGon::OnRender()
	{
		switch (mDrawStrategy)
		{
		case DrawStrategy::Points:
			RenderPoints();
			break;
		case DrawStrategy::Lines:
			RenderLines();
			break;
		}
	}

	void LabTask_DrawNGon::RenderPoints()
	{
		mShaders.BindToPipeline();
		mNGonGeometry.BindToPipeline();
		glPointSize(mPointSize);
		(mSmoothPoint ? glEnable : glDisable)(GL_POINT_SMOOTH);
		glDrawArrays(GL_POINTS, 0, mNGon.size());
	}

	void LabTask_DrawNGon::RenderLines()
	{
		mShaders.BindToPipeline();
		mNGonGeometry.BindToPipeline();
		glLineWidth(mLineWidth);
		glDrawArrays(GL_LINE_STRIP, 0, mNGon.size() + 1);
	}

	void LabTask_DrawNGon::LoadShaders()
	{
		mShaders = OglProgramPipeline(OglProgramPipeline::Params{ "NGon shaders" });

		mNGonShaders[ShaderStage::Vertex].FetchFromFile("glsl/ngon_vert.glsl", "ngon-vert");
		mNGonShaders[ShaderStage::Fragment].FetchFromFile("glsl/ngon_frag.glsl", "ngon-frag");

		mShaders[ShaderStage::Vertex].Set(mNGonShaders[ShaderStage::Vertex].FromName("ngon-vert"));
		mShaders[ShaderStage::Fragment].Set(mNGonShaders[ShaderStage::Fragment].FromName("ngon-frag"));
	}

	void LabTask_DrawNGon::RegenerateNGon()
	{
		mNGon = CalculateNGonCorners(mNGonSize, mNGonRadius, mNGonCenter);

		switch (mDrawStrategy)
		{
		case DrawStrategy::Points:
			LoadNGonAsPoints();
			break;
		case DrawStrategy::Lines:
			LoadNGonAsLines();
			break;
		}
	}

	void LabTask_DrawNGon::LoadNGonAsPoints()
	{
		mNGonPositions = OglBuffer(OglBuffer::Params{
			"NGon point positions",
			GL_ARRAY_BUFFER,
			sizeof(Vector2f),
			mNGon.size()
		});
		mNGonPositions.SetData(mNGon.data());

		OglGeometryInput::Params ngonAsPoints;
		ngonAsPoints.DebugName = "NGon layout as points";
		ngonAsPoints.VertexBuffers[0] = &mNGonPositions;
		ngonAsPoints.IndexBuffer = nullptr;
		ngonAsPoints.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};

		mNGonGeometry = OglGeometryInput(std::move(ngonAsPoints));
	}

	void LabTask_DrawNGon::LoadNGonAsLines()
	{
		std::vector<Vector2f> ngon = mNGon;
		ngon.push_back(ngon.front());

		mNGonPositions = OglBuffer(OglBuffer::Params{
			"NGon lines junction positions",
			GL_ARRAY_BUFFER,
			sizeof(Vector2f),
			ngon.size()
		});
		mNGonPositions.SetData(ngon.data());

		OglGeometryInput::Params ngonAsLines;
		ngonAsLines.DebugName = "NGon layout as lines";
		ngonAsLines.VertexBuffers[0] = &mNGonPositions;
		ngonAsLines.IndexBuffer = nullptr;
		ngonAsLines.Vertices = {
			VertexParams{ 0, VertexFormat::Float2 }
		};

		mNGonGeometry = OglGeometryInput(std::move(ngonAsLines));
	}
}

