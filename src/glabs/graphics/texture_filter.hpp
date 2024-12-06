#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	enum class TextureFilter
	{
		Point,
		Linear,
	};

	constexpr GLenum GetNativeTextureFilter(TextureFilter filter)
	{
		switch (filter)
		{
		case TextureFilter::Point:
			return GL_NEAREST;
		case TextureFilter::Linear:
			return GL_LINEAR;
		default:
			return GL_FALSE;
		}
	}
}

