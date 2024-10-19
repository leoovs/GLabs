#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	enum class MeshAttribute
	{
		None = 0x00,
		Position = 0x01,
		Normal = 0x02,
		UV = 0x03,
	};

	MeshAttribute operator|(MeshAttribute left, MeshAttribute right);
	MeshAttribute operator|=(MeshAttribute& left, MeshAttribute right);

	MeshAttribute operator&(MeshAttribute left, MeshAttribute right);
	MeshAttribute operator&=(MeshAttribute& left, MeshAttribute right);

	size_t CalculateVertexSize(MeshAttribute attributes);
}

