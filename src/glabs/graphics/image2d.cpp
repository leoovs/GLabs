#include "glabs/graphics/image2d.hpp"

namespace glabs
{
	Image2D Image2D::FromFile(std::string_view fileName, int32_t forceChannels)
	{
		int width = 0;
		int height = 0;
		int channels = 0;
		stbi_uc* imageData = stbi_load(fileName.data(), &width, &height, &channels, forceChannels);

		if (nullptr == imageData)
		{
			return Image2D();
		}

		Image2D image(width, height, channels);
		std::memcpy(image.GetPixels(), imageData, size_t(width) * height * channels);

		return image;
	}

	Image2D::Image2D(int32_t width, int32_t height, int32_t channels)
		: mPixels(size_t(width) * height * channels)
		, mWidth(width)
		, mHeight(height)
		, mChannels(channels)
	{}

	std::byte* Image2D::GetPixels()
	{
		return mPixels.data();
	}

	const std::byte* Image2D::GetPixels() const
	{
		return mPixels.data();
	}

	int32_t Image2D::GetWidth() const
	{
		return mWidth;
	}

	int32_t Image2D::GetHeight() const
	{
		return mHeight;
	}

	int32_t Image2D::GetChannels() const
	{
		return mChannels;
	}
}
