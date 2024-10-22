#pragma once

#include "glabs/pch.hpp"
#include "glabs/rendering/mesh.hpp"
#include "glabs/rendering/mesh_attribute.hpp"

namespace glabs
{
	class ObjImporter
	{
	public:
		ObjImporter& OpenFile(const std::filesystem::path& meshFilePath);
		ObjImporter& LoadShape(std::string_view shapeName);
		ObjImporter& LoadAllShapes();
		Mesh Build();

	private:
		enum class State
		{
			Uncommited,
			ObjLoaded,
			FailedToLoadObj,
			Committed,
		};

		static std::vector<VertexParams> GetVertexParamsFromAttributes(
			MeshAttribute attributes
		);

		void LoadShapeByIndex(size_t shapeIndex);

		Mesh mImportedMesh;
		State mState = State::Uncommited;

		tinyobj::attrib_t mAttributes;
		std::vector<tinyobj::shape_t> mShapes;
		std::vector<bool> mShapeIsLoaded;
	};
}

