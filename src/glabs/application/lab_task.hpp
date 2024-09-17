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
		DrawNGonTriangleFan, // Задание 6
		DrawThirdShape, // Задание 7-8
		Count_,
	};

	constexpr const char* GetLabTaskKindUserFriendlyString(LabTaskKind kind)
	{
		switch (kind)
		{
		case LabTaskKind::None:
			return "Нет задания";
		case LabTaskKind::DrawNGon:
			return "Задание 1-2";
		case LabTaskKind::DrawFirstShape:
			return "Задание 3";
		case LabTaskKind::DrawSecondShapeLineStrip:
			return "Задание 4";
		case LabTaskKind::DrawSecondShapeTriangles:
			return "Задание 5";
		case LabTaskKind::DrawNGonTriangleFan:
			return "Задание 6";
		case LabTaskKind::DrawThirdShape:
			return "Задание 7-8";
		default:
			return "???";
		}
	}

	class LabTask
	{
	public:
		virtual ~LabTask() = default;

		virtual LabTaskKind GetKind() const = 0;
		virtual LabTaskKind GetNext() const = 0;
		virtual LabTaskKind GetPrev() const = 0;

		virtual void OnEnter() = 0;
		virtual void OnQuit() = 0;

		virtual void OnMainMenu() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
	};
}

