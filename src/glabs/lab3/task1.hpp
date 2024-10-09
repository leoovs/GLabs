#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/lab3/task.hpp"

#include <glm/ext.hpp>
#include <glm/vec2.hpp>

namespace glabs
{
	class Task1 : public Task
	{
	public:
		enum class SubtaskName : int
		{
			A,
			B,
			C,
			D,
			Count_,
		};

		static std::string_view SubtaskToStr(SubtaskName name);

		Task1();
		~Task1() override = default;

		TaskName GetName() const override;
		TaskName GetNext() const override;
		TaskName GetPrev() const override;

		std::string_view GetTitle() const override;

		void OnMainMenu() override;
		void OnRender() override;

	private:
		static constexpr size_t cNGonSize = 6;
		static constexpr float cNGonRadius = 0.5f;
		using NGon = std::array<glm::vec2, cNGonSize>;

		NGon GenerateNGon();

		void LoadNGonPositions();

		SubtaskName mCurrentSubtask = SubtaskName::A;

		OglBuffer mNGonPositions;
		OglBuffer mNGonIndices;
		OglGeometryInput mNGonGeometry;
	};
}

