#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"

namespace glabs
{
	class Mesh
	{
	public:
		enum class Attributes
		{
			None = 0x0,

			Position = 0x1,
			Normal = 0x2,
			UV = 0x4,

			PosNorm = Position | Normal,
			PosUV = Position | UV,
			NormUV = Normal | UV,
			PosNormUV = Position | Normal | UV,
		};

		static size_t CalculateVertexByteWidth(Attributes attribFlags);

		static Mesh MakeCube(float side = 0.5f);
		static Mesh FromObjFile(const std::filesystem::path& meshFile);

		Mesh() = default;

		Mesh(
			OglBuffer vertices,
			OglBuffer indices,
			Attributes attribs,
			std::string name = "Unnamed Mesh"
		);

		size_t GetIndexCount() const;
		size_t GetVertexCount() const;
		std::string_view GetName() const;

		void SetName(std::string name);

		void BindToPipeline() const;

	private:
		void CreateGeometryFromAttribs();

		OglBuffer mVertices;
		OglBuffer mIndices;
		Attributes mAttribs;
		std::string mName;
		OglGeometryInput mGeometry;
	};

	constexpr Mesh::Attributes operator|(Mesh::Attributes left, Mesh::Attributes right)
	{
		return static_cast<Mesh::Attributes>(
			static_cast<std::underlying_type_t<Mesh::Attributes>>(left)
				| static_cast<std::underlying_type_t<Mesh::Attributes>>(right)
		);
	}

	constexpr Mesh::Attributes operator&(Mesh::Attributes left, Mesh::Attributes right)
	{
		return static_cast<Mesh::Attributes>(
			static_cast<std::underlying_type_t<Mesh::Attributes>>(left)
				& static_cast<std::underlying_type_t<Mesh::Attributes>>(right)
		);
	}

	constexpr Mesh::Attributes operator|=(Mesh::Attributes& left, Mesh::Attributes right)
	{
		return left = left | right;
	}

	constexpr Mesh::Attributes operator&=(Mesh::Attributes& left, Mesh::Attributes right)
	{
		return left = left & right;
	}
}

