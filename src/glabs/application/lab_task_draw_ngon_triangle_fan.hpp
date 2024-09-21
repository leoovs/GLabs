#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/rendering/shader_library.hpp"
#pragma ocne

#include "glabs/application/lab_task.hpp"

namespace glabs
{
	class LabTask_DrawNGonTriangleFan : public LabTask
	{
	public:
		LabTask_DrawNGonTriangleFan();
		~LabTask_DrawNGonTriangleFan() override = default;

		LabTaskKind GetKind() const override;
		LabTaskKind GetNext() const override;
		LabTaskKind GetPrev() const override;

		void OnEnter() override;
		void OnQuit() override;

		void OnMainMenu() override;
		void OnUpdate() override;
		void OnRender() override;

	private:
		void GenerateNGon();
		void LoadShaders();

		static constexpr int cNGonMinSize = 3;
		static constexpr int cNGonMaxSize = 10;
		int mNGonSize = cNGonMinSize;

		OglBuffer mNGonPositions;
		OglGeometryInput mNGonGeometry;
		OglProgramPipeline mPrograms;
		ShaderLibrary mNGonShaders;
	};
}

