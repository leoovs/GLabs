#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	enum class GraphicsFormat
	{
		R8G8B8_UNORM,
		R8G8B8_SNORM,
		R8G8B8A8_UNORM,
		R16_FLOAT,
		R16G16_FLOAT,
		R16G16B16_FLOAT,
		D32_UNORM,
	};

	constexpr GLenum GetInternalFormat(GraphicsFormat format)
	{
		switch (format)
		{
		case GraphicsFormat::R8G8B8_UNORM:
			return GL_RGB8;
		case GraphicsFormat::R8G8B8_SNORM:
			return GL_RGB8;
		case GraphicsFormat::R8G8B8A8_UNORM:
			return GL_RGBA8;
		case GraphicsFormat::R16_FLOAT:
			return GL_R16F;
		case GraphicsFormat::R16G16_FLOAT:
			return GL_RG16F;
		case GraphicsFormat::R16G16B16_FLOAT:
			return GL_RGB16F;
		case GraphicsFormat::D32_UNORM:
			return GL_DEPTH_COMPONENT32;
		default:
			return 0;
		}
	}

	constexpr GLenum GetFormatType(GraphicsFormat format)
	{
		switch (format)
		{
		case GraphicsFormat::R8G8B8_UNORM:
		case GraphicsFormat::R8G8B8A8_UNORM:
			return GL_UNSIGNED_BYTE;
		case GraphicsFormat::R8G8B8_SNORM:
			return GL_BYTE;
		case GraphicsFormat::R16_FLOAT:
			[[fallthrough]];
		case GraphicsFormat::R16G16_FLOAT:
			[[fallthrough]];
		case GraphicsFormat::R16G16B16_FLOAT:
			return GL_HALF_FLOAT;
		case GraphicsFormat::D32_UNORM:
			return GL_UNSIGNED_INT;
		default:
			return 0;
		}
	}

	constexpr GLenum GetNativeFormat(GraphicsFormat format)
	{
		switch (format)
		{
		case GraphicsFormat::R8G8B8_UNORM:
			[[fallthrough]];
		case GraphicsFormat::R8G8B8_SNORM:
			return GL_RGB;
		case GraphicsFormat::R8G8B8A8_UNORM:
			return GL_RGBA;
		case GraphicsFormat::R16_FLOAT:
			return GL_R;
		case GraphicsFormat::R16G16_FLOAT:
			return GL_RG;
		case GraphicsFormat::R16G16B16_FLOAT:
			return GL_RGB;
		case GraphicsFormat::D32_UNORM:
			return GL_DEPTH_COMPONENT;
		default:
			return 0;
		}
	}

	inline int32_t CalculateMipLevels(int32_t width, int32_t height)
	{
		return std::log2(std::max(width, height)) + 1;
	}
}

