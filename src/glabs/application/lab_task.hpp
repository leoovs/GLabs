#pragma once

namespace glabs
{
	enum class LabTaskKind
	{
		None = -1,
		DrawNGon, // Задание 1-2
		DrawFirstShape, // Задание 3
		DrawSecondShapeLineStrip, // Задание 4
		DrawSecondShapeTriangles, // Задание 5
		DrawNGonTriangleStrip, // Задание 6
		DrawThirdShape, // Задание 7-8
		Count_,
	};

	class LabTask
	{
	public:
		virtual ~LabTask() = default;

		virtual LabTaskKind GetKind() const = 0;

		virtual void OnEnter() = 0;
		virtual void OnQuit() = 0;

		virtual void OnMainMenu() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
	};
}

