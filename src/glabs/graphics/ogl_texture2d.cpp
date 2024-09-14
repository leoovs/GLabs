#include "glabs/graphics/ogl_texture2d.hpp"

namespace glabs
{
	OglTexture2D::OglTexture2D(Params params)
		: mParams(std::move(params))
	{
		CreateNativeTexture2D();
	}

	OglTexture2D::OglTexture2D(OglTexture2D&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeTexture2D(std::exchange(other.mNativeTexture2D, 0))
	{}

	OglTexture2D& OglTexture2D::operator=(OglTexture2D&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeTexture2D();
		mParams = std::move(other.mParams);
		mNativeTexture2D = std::exchange(other.mNativeTexture2D, 0);

		return *this;
	}

	OglTexture2D::~OglTexture2D()
	{
		DestroyNativeTexture2D();
	}

	const OglTexture2D::Params& OglTexture2D::GetParams() const
	{
		return mParams;
	}

	GLuint OglTexture2D::GetNativeTexture2D() const
	{
		return mNativeTexture2D;
	}

	void OglTexture2D::SetData(const void* data)
	{
		glTextureSubImage2D(
			mNativeTexture2D,
			0,
			0,
			0,
			GLsizei(mParams.Width),
			GLsizei(mParams.Height),
			GetNativeFormat(mParams.Format),
			GetFormatType(mParams.Format),
			data
		);
	}

	void OglTexture2D::BindToPipeline(int32_t unit)
	{
		glBindTextureUnit(GLuint(unit), mNativeTexture2D);
	}

	void OglTexture2D::CreateNativeTexture2D()
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &mNativeTexture2D);
		glTextureStorage2D(
			mNativeTexture2D,
			GLsizei(mParams.MipLevels),
			GetInternalFormat(mParams.Format),
			GLsizei(mParams.Width),
			GLsizei(mParams.Height)
		);
	}

	void OglTexture2D::DestroyNativeTexture2D()
	{
		if (0 != mNativeTexture2D)
		{
			glDeleteTextures(1, &mNativeTexture2D);
		}
	}
}

