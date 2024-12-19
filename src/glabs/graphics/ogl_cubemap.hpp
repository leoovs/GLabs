#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/graphics_format.hpp"

namespace glabs
{
	enum class CubemapFace;

	class OglCubemap
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglCubemap";

			int32_t SideSize = 8;
			int32_t MipLevels = 1;
			int32_t ArraySize = 1;
			GraphicsFormat Format = GraphicsFormat::R8G8B8_UNORM;
		};

		OglCubemap() = default;
		OglCubemap(Params params);

		OglCubemap(OglCubemap&& other) noexcept;

		OglCubemap& operator=(OglCubemap&& other) noexcept;

		~OglCubemap();

		const Params& GetParams() const;
		GLuint GetNativeCubemap() const;

		void SetData(const void* data, CubemapFace face, int32_t arrayIndex = 0);

		void GenerateMipMaps();
		void BindToPipeline(int32_t textureUnit);

	private:
		void CreateNativeCubemap();
		void DestroyNativeCubemap();

		Params mParams;
		GLuint mNativeCubemap = 0;
	};
}

