#include "glabs/graphics/ogl_cubemap.hpp"

#include "glabs/pch.hpp"
#include "glabs/graphics/cubemap_face.hpp"

namespace glabs
{
	OglCubemap::OglCubemap(Params params)
		: mParams(std::move(params))
	{
		CreateNativeCubemap();
	}

	OglCubemap::OglCubemap(OglCubemap&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeCubemap(std::exchange(other.mNativeCubemap, 0))
	{}

	OglCubemap& OglCubemap::operator=(OglCubemap&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeCubemap();

		mParams = std::move(other.mParams);
		mNativeCubemap = std::exchange(other.mNativeCubemap, 0);

		return *this;
	}

	OglCubemap::~OglCubemap()
	{
		DestroyNativeCubemap();
	}

	const OglCubemap::Params& OglCubemap::GetParams() const
	{
		return mParams;
	}

	GLuint OglCubemap::GetNativeCubemap() const
	{
		return mNativeCubemap;
	}

	void OglCubemap::SetData(const void* data, CubemapFace face, int32_t arrayIndex)
	{
		auto faceIndex = static_cast<GLuint>(CubemapFaceToNative(face));

		glTextureSubImage3D(
			mNativeCubemap,
			0,
			0,
			0,
			arrayIndex * size_t(CubemapFace::Count_) + faceIndex,
			mParams.SideSize,
			mParams.SideSize,
			1,
			GetNativeFormat(mParams.Format),
			GetFormatType(mParams.Format),
			data
		);
	}

	void OglCubemap::GenerateMipMaps()
	{
		glGenerateTextureMipmap(mNativeCubemap);
	}

	void OglCubemap::BindToPipeline(int32_t textureUnit)
	{
		glBindTextureUnit(GLuint(textureUnit), mNativeCubemap);
	}

	void OglCubemap::CreateNativeCubemap()
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &mNativeCubemap);

		glObjectLabel(
			GL_TEXTURE,
			mNativeCubemap,
			GLsizei(mParams.DebugName.length()),
			mParams.DebugName.data()
		);

		glTextureStorage3D(
			mNativeCubemap,
			GLsizei(mParams.MipLevels),
			GetInternalFormat(mParams.Format),
			mParams.SideSize,
			mParams.SideSize,
			size_t(CubemapFace::Count_) * mParams.ArraySize
		);
	}

	void OglCubemap::DestroyNativeCubemap()
	{
		if (0 != mNativeCubemap)
		{
			glDeleteTextures(1, &mNativeCubemap);
			mNativeCubemap = 0;
		}
	}
}

