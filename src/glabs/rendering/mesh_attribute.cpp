#include "glabs/rendering/mesh_attribute.hpp"

namespace glabs
{
	using MeshAttributeFlags = std::underlying_type_t<MeshAttribute>;

	MeshAttribute operator|(MeshAttribute left, MeshAttribute right)
	{
		return static_cast<MeshAttribute>(
			static_cast<MeshAttributeFlags>(left)
				| static_cast<MeshAttributeFlags>(right)
		);
	}

	MeshAttribute operator|=(MeshAttribute& left, MeshAttribute right)
	{
		return left = left | right;
	}

	MeshAttribute operator&(MeshAttribute left, MeshAttribute right)
	{
		return static_cast<MeshAttribute>(
			static_cast<MeshAttributeFlags>(left)
				& static_cast<MeshAttributeFlags>(right)
		);
	}

	MeshAttribute operator&=(MeshAttribute& left, MeshAttribute right)
	{
		return left = left & right;
	}

	size_t CalculateVertexSize(MeshAttribute attributes)
	{
		size_t size = 0;

		if (MeshAttribute::None != (attributes & MeshAttribute::Position))
		{
			size += sizeof(float[3]);
		}

		if (MeshAttribute::None != (attributes & MeshAttribute::Normal))
		{
			size += sizeof(float[3]);
		}

		if (MeshAttribute::None != (attributes & MeshAttribute::UV))
		{
			size += sizeof(float[2]);
		}

		return size;
	}
}

