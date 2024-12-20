#include "glabs/graphics/ogl_texture2d.hpp"

namespace glabs
{
	OglTexture2D::OglTexture2D(Params params)
		: mParams(std::move(params))
	{
		CreateNativeTexture2DArray();
	}

	OglTexture2D::OglTexture2D(OglTexture2D&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeTexture2DArray(std::exchange(other.mNativeTexture2DArray, 0))
	{}

	OglTexture2D& OglTexture2D::operator=(OglTexture2D&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeTexture2DArray();
		mParams = std::move(other.mParams);
		mNativeTexture2DArray = std::exchange(other.mNativeTexture2DArray, 0);

		return *this;
	}

	OglTexture2D::~OglTexture2D()
	{
		DestroyNativeTexture2DArray();
	}

	const OglTexture2D::Params& OglTexture2D::GetParams() const
	{
		return mParams;
	}

	GLuint OglTexture2D::GetNativeTexture2D() const
	{
		return mNativeTexture2DArray;
	}

	void OglTexture2D::SetData(const void* data, int32_t arrayIndex)
	{
		assert(0 != mNativeTexture2DArray);

		glTextureSubImage3D(
			mNativeTexture2DArray,
			0,
			0,
			0,
			arrayIndex,
			GLsizei(mParams.Width),
			GLsizei(mParams.Height),
			1,
			GetNativeFormat(mParams.Format),
			GetFormatType(mParams.Format),
			data
		);
	}

	void OglTexture2D::GenerateMipMaps()
	{
		glGenerateTextureMipmap(mNativeTexture2DArray);
	}

	void OglTexture2D::BindToPipeline(int32_t unit) const
	{
		glBindTextureUnit(GLuint(unit), mNativeTexture2DArray);
	}

	void OglTexture2D::CreateNativeTexture2DArray()
	{
		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &mNativeTexture2DArray);

		glObjectLabel(
			GL_TEXTURE,
			mNativeTexture2DArray,
			GLsizei(mParams.DebugName.length()),
			mParams.DebugName.data()
		);

		glTextureStorage3D(
			mNativeTexture2DArray,
			mParams.MipLevels,
			GetInternalFormat(mParams.Format),
			mParams.Width,
			mParams.Height,
			mParams.ArraySize
		);
	}

	void OglTexture2D::DestroyNativeTexture2DArray()
	{
		if (0 != mNativeTexture2DArray)
		{
			glDeleteTextures(1, &mNativeTexture2DArray);
			mNativeTexture2DArray = 0;
		}
	}
}

