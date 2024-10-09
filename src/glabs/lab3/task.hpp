#pragma once

namespace glabs
{
	enum class TaskName
	{
		None = -1,
		Task1,
		Task2,
		Task3,
		Count_,
	};

	class Task
	{
	public:
		virtual ~Task() = default;

		virtual TaskName GetName() const = 0;
		virtual TaskName GetNext() const = 0;
		virtual TaskName GetPrev() const = 0;

		virtual std::string_view GetTitle() const = 0;

		virtual void OnMainMenu() {}
		virtual void OnRender() {}
	};
}

