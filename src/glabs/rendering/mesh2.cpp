#include "glabs/rendering/mesh2.hpp"

namespace glabs
{
	Mesh2::Mesh2(std::string_view name)
		: mName(name)
	{}

	void Mesh2::SetName(std::string_view name)
	{
		mName = name;
	}

	std::string_view Mesh2::GetName() const
	{
		return mName;
	}

	void Mesh2::Insert(Submesh shape)
	{
		mShapes.push_back(std::move(shape));
	}

	Submesh& Mesh2::FindShape(std::string_view name)
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

	const std::vector<Submesh>& Mesh2::GetShapes() const
	{
		return mShapes;
	}
}

