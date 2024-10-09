#include "glabs/lab3/task1.hpp"

namespace glabs
{
	std::string_view Task1::SubtaskToStr(SubtaskName name)
	{
		switch (name)
		{
		case SubtaskName::A:
			return "а)";
		case SubtaskName::B:
			return "б)";
		case SubtaskName::C:
			return "в)";
		case SubtaskName::D:
			return "г)";
		default:
			return "???";
		}
	}

	Task1::Task1()
	{
		LoadNGonPositions();
	}

	TaskName Task1::GetName() const
	{
		return TaskName::Task1;
	}

	TaskName Task1::GetNext() const
	{
		return TaskName::Task2;
	}

	TaskName Task1::GetPrev() const
	{
		return TaskName::Task3;
	}

	std::string_view Task1::GetTitle() const
	{
		return "Задание 1";
	}

	void Task1::OnMainMenu()
	{
		bool subtaskChanged = ImGui::Combo(
			"Подзадание",
			reinterpret_cast<int*>(&mCurrentSubtask),
			[](void* userdata, int index) { return SubtaskToStr(SubtaskName(index)).data(); },
			nullptr,
			int(SubtaskName::Count_)
		);
	}

	void Task1::OnRender()
	{
	}

	Task1::NGon Task1::GenerateNGon()
	{
		NGon ngon;
		constexpr float cAngle = 2.0f * glm::pi<float>() / ngon.size();

		for (size_t iSide = 0; iSide < ngon.size(); iSide++)
		{
			ngon.at(iSide) = glm::vec2
			{
				cNGonRadius *	std::cosf(iSide * cAngle),
				cNGonRadius *	std::sinf(iSide * cAngle),
			};
		}

		return ngon;
	}

	void Task1::LoadNGonPositions()
	{
		NGon positions = GenerateNGon();

		OglBuffer::Params ngonPositionsParams;
		ngonPositionsParams.DebugName = "NGon positions";
		ngonPositionsParams.Target = GL_ARRAY_BUFFER;
		ngonPositionsParams.ElementCount = cNGonSize;
		ngonPositionsParams.ElementSize = sizeof(glm::vec2);

		mNGonPositions = OglBuffer(std::move(ngonPositionsParams));
		mNGonPositions.SetData(positions.data());
	}
}

