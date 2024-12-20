#include "glabs/rendering/material.hpp"
#include "glabs/graphics/image2d.hpp"

namespace glabs
{
	Material& Material::LoadFromFile(
		MaterialKind kind,
		std::string_view filename,
		GraphicsFormat format
	)
	{

		Material& self = *this;
		Image2D image = Image2D::FromFile(filename);

		OglTexture2D::Params mapParams;
		mapParams.DebugName = "Material - " + std::string(GetMaterialKindName(kind));
		mapParams.Width = image.GetWidth();
		mapParams.Height = image.GetHeight();
		mapParams.Format = format;
		mapParams.MipLevels = CalculateMipLevels(image.GetWidth(), image.GetHeight());

		OglTexture2D map(std::move(mapParams));
		map.SetData(image.GetPixels());
		map.GenerateMipMaps();

		self[kind] = std::move(map);

		return *this;
	}

	OglTexture2D& Material::operator[](MaterialKind kind)
	{
		return mMaps.at(size_t(kind));
	}

	const OglTexture2D& Material::operator[](MaterialKind kind) const
	{
		return mMaps.at(size_t(kind));
	}
}

