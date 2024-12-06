#pragma once

#include "glabs/graphics/texture_filter.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	class OglSampler
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglSampler";

			TextureFilter MinimizingFilter = TextureFilter::Linear;
			TextureFilter MagnifyingFilter = TextureFilter::Point;
			bool SeamlessCubemaps = true;
		};

		OglSampler() = default;
		OglSampler(Params params);

		OglSampler(OglSampler&& other) noexcept;

		OglSampler& operator=(OglSampler&& other) noexcept;

		~OglSampler();

		const Params& GetParams() const;
		GLuint GetNativeSampler() const;

		void BindToPipeline(int32_t samplerUnit) const;

	private:
		void CreateNativeSampler();
		void DestroyNativeSampler();

		Params mParams;
		GLuint mNativeSampler = 0;
	};
}

