#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	enum class CubemapFace
	{
		Right,
		Left,
		Top,
		Bottom,
		Back,
		Front,

		Last_,

		Count_ = Last_,
	};

	constexpr int32_t CubemapFaceToNative(CubemapFace face)
	{
		return static_cast<int32_t>(face);
	}
}

