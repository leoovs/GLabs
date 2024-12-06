#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/graphics_format.hpp"

namespace glabs
{
	class OglTexture2D
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglTexture2D";

			int32_t Width = 8;
			int32_t Height = 8;
			int32_t MipLevels = 1;
			int32_t ArraySize = 1;
			GraphicsFormat Format = GraphicsFormat::R8G8B8_UNORM;
		};

		OglTexture2D() = default;
		OglTexture2D(Params params);

		OglTexture2D(OglTexture2D&& other) noexcept;

		OglTexture2D& operator=(OglTexture2D&& other) noexcept;

		~OglTexture2D();

		const Params& GetParams() const;
		GLuint GetNativeTexture2D() const;

		void SetData(const void* data, int32_t arrayIndex = 0);

		void GenerateMipMaps();
		void BindToPipeline(int32_t unit);

	private:
		void CreateNativeTexture2DArray();
		void DestroyNativeTexture2DArray();

		Params mParams;
		GLuint mNativeTexture2DArray = 0;
	};
}

