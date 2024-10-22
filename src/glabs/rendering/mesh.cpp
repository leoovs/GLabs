#include "glabs/rendering/mesh.hpp"

namespace glabs
{
	Mesh::Mesh(std::string_view name)
		: mName(name)
	{}

	void Mesh::SetName(std::string_view name)
	{
		mName = name;
	}

	std::string_view Mesh::GetName() const
	{
		return mName;
	}

	void Mesh::Insert(Submesh shape)
	{
		mShapes.push_back(std::move(shape));
	}

	Submesh& Mesh::FindShape(std::string_view name)
	{
		auto it = std::find_if(
			mShapes.begin(),
			mShapes.end(),
			[name](const Submesh& shape)
			{
				return shape.GetName() == name;
			}
		);

		assert(mShapes.end() != it);

		return *it;
	}
}

