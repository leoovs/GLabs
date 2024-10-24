#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	enum class TaskName
	{
		None = -1,
		Task1,
		Task3,

		Count_,
	};

	constexpr inline size_t operator+(TaskName value)
	{
		return static_cast<size_t>(value);
	}

	constexpr std::string_view TaskNameToStr(TaskName task)
	{
		switch (task)
		{
		case TaskName::Task1:
			return "Задание 1, 2";
		case TaskName::Task3:
			return "Задание 3, 4, 5, 6";
		default:
			return "???";
		}
	}

	class Task
	{
	public:
		virtual ~Task() = default;

		virtual void OnUI() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnRender() {}

		virtual TaskName GetName() const = 0;
		virtual TaskName GetNext() const = 0;
		virtual TaskName GetPrev() const = 0;
	};
}

