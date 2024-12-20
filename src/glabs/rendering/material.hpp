#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"

namespace glabs
{
	enum class MaterialKind
	{
		Albedo,
		Normal,
		Specular,
		Count_
	};

	constexpr std::string_view GetMaterialKindName(MaterialKind kind)
	{
		switch (kind)
		{
		case MaterialKind::Albedo:
			return "Albedo";
		case MaterialKind::Normal:
			return "Normal";
		case MaterialKind::Specular:
			return "Specular";
		default:
			return "???";
		}
	}

	constexpr GraphicsFormat GetMaterialStorageFormat(MaterialKind kind)
	{
		switch (kind)
		{
		case MaterialKind::Albedo:
			return GraphicsFormat::R8G8B8_UNORM;
		case MaterialKind::Normal:
			return GraphicsFormat::R8G8B8_UNORM;
		case MaterialKind::Specular:
			return GraphicsFormat::R8G8B8_UNORM;
		default:
			return {};
		}
	}

	class Material
	{
	public:
		Material& LoadFromFile(
			MaterialKind kind,
			std::string_view filename,
			GraphicsFormat format = GraphicsFormat::R8G8B8_SNORM
		);

		OglTexture2D& operator[](MaterialKind kind);
		const OglTexture2D& operator[](MaterialKind kind) const;

	private:
		std::array<OglTexture2D, size_t(MaterialKind::Count_)> mMaps;
	};
}
