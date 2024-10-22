#pragma once

#include "glabs/rendering/sub_mesh.hpp"

namespace glabs
{
	class Mesh
	{
	public:
		Mesh(std::string_view name = "Unnamed Mesh");

		void SetName(std::string_view name);
		std::string_view GetName() const;

		void Insert(Submesh shape);
		Submesh& FindShape(std::string_view name);

		template<typename CallableT>
		void ForEachShape(const CallableT& callable) const
		{
			for (const Submesh& shape : mShapes)
			{
				callable(shape);
			}
		}

	private:
		std::string mName;
		std::vector<Submesh> mShapes;
	};
}

