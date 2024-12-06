#include "glabs/graphics/ogl_sampler.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	OglSampler::OglSampler(Params params)
		: mParams(std::move(params))
	{
		CreateNativeSampler();
	}

	OglSampler::OglSampler(OglSampler&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeSampler(std::exchange(other.mNativeSampler, 0))
	{}

	OglSampler& OglSampler::operator=(OglSampler&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeSampler();

		mParams = std::move(other.mParams);
		mNativeSampler = std::exchange(other.mNativeSampler, 0);

		return *this;
	}

	OglSampler::~OglSampler()
	{
		DestroyNativeSampler();
	}

	const OglSampler::Params& OglSampler::GetParams() const
	{
		return mParams;
	}

	GLuint OglSampler::GetNativeSampler() const
	{
		return mNativeSampler;
	}

	void OglSampler::BindToPipeline(int32_t samplerUnit) const
	{
		glBindSampler(GLuint(samplerUnit), mNativeSampler);
	}

	void OglSampler::CreateNativeSampler()
	{
		glCreateSamplers(1, &mNativeSampler);

		glObjectLabel(
			GL_SAMPLER,
			mNativeSampler,
			GLsizei(mParams.DebugName.length()),
			mParams.DebugName.data()
		);

		glSamplerParameteri(mNativeSampler, GL_TEXTURE_MAG_FILTER, GetNativeTextureFilter(mParams.MagnifyingFilter));
		glSamplerParameteri(mNativeSampler, GL_TEXTURE_MIN_FILTER, GetNativeTextureFilter(mParams.MinimizingFilter));
		glSamplerParameteri(mNativeSampler, GL_TEXTURE_CUBE_MAP_SEAMLESS, GLint(mParams.SeamlessCubemaps));
	}

	void OglSampler::DestroyNativeSampler()
	{
		if (0 != mNativeSampler)
		{
			glDeleteSamplers(1, &mNativeSampler);
			mNativeSampler = 0;
		}
	}
}

