#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class Image2D
	{
	public:
		static Image2D FromFile(std::string_view fileName, int32_t forceChannels = 0);

		Image2D() = default;
		Image2D(int32_t width, int32_t height, int32_t channels);

		std::byte* GetPixels();
		const std::byte* GetPixels() const;

		int32_t GetWidth() const;
		int32_t GetHeight() const;
		int32_t GetChannels() const;

	private:
		std::vector<std::byte> mPixels;
		int32_t mWidth = 0;
		int32_t mHeight = 0;
		int32_t mChannels = 0;
	};
}

