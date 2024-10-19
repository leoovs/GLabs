#pragma once

#include "glabs/rendering/sub_mesh.hpp"

namespace glabs
{
	class Mesh2
	{
	public:
		Mesh2(std::string_view name = "Unnamed Mesh");

		void SetName(std::string_view name);
		std::string_view GetName() const;

		void Insert(Submesh shape);
		Submesh& FindShape(std::string_view name);
		const std::vector<Submesh>& GetShapes() const;

	private:
		std::string mName;
		std::vector<Submesh> mShapes;
	};
}

