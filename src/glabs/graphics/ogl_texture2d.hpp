#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	enum class GraphicsFormat
	{
		R8G8B8_UNORM,
		R8G8B8A8_UNORM,
	};

	constexpr GLenum GetInternalFormat(GraphicsFormat format)
	{
		switch (format)
		{
		case GraphicsFormat::R8G8B8_UNORM:
			return GL_RGB8;
		case GraphicsFormat::R8G8B8A8_UNORM:
			return GL_RGBA8;
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
		default:
			return 0;
		}
	}

	constexpr GLenum GetNativeFormat(GraphicsFormat format)
	{
		switch (format)
		{
		case GraphicsFormat::R8G8B8_UNORM:
			return GL_RGB;
		case GraphicsFormat::R8G8B8A8_UNORM:
			return GL_RGBA;
		default:
			return 0;
		}
	}

	class OglTexture2D
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglTexture2D";

			int32_t Width = 8;
			int32_t Height = 8;
			int32_t MipLevels = 1;
			GraphicsFormat Format = GraphicsFormat::R8G8B8_UNORM;
		};

		static constexpr int32_t CalculateMipLevels(int32_t width, int32_t height)
		{
			return std::log2(std::max(width, height)) + 1;
		}

		OglTexture2D() = default;
		OglTexture2D(Params params);

		OglTexture2D(OglTexture2D&& other) noexcept;

		OglTexture2D& operator=(OglTexture2D&& other) noexcept;

		~OglTexture2D();

		const Params& GetParams() const;
		GLuint GetNativeTexture2D() const;

		void SetData(const void* data);

		void GenerateMipMaps();
		void BindToPipeline(int32_t unit);

	private:
		void CreateNativeTexture2D();
		void DestroyNativeTexture2D();

		Params mParams;
		GLuint mNativeTexture2D = 0;
	};
}

