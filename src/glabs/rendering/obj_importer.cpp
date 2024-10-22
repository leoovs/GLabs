#include "glabs/rendering/obj_importer.hpp"
#include "glabs/rendering/mesh_attribute.hpp"

namespace glabs
{
	ObjImporter& ObjImporter::OpenFile(const std::filesystem::path& meshFilePath)
	{
		assert(State::Uncommited == mState);

		bool loaded = tinyobj::LoadObj(&mAttributes, &mShapes, nullptr, nullptr, meshFilePath.string().c_str());

		mState = loaded
			? State::ObjLoaded
			: State::FailedToLoadObj;

		if (loaded)
		{
			mShapeIsLoaded.resize(mShapes.size(), false);
		}

		return *this;
	}

	ObjImporter& ObjImporter::LoadShape(std::string_view shapeName)
	{
		assert(State::ObjLoaded == mState);

		for (size_t iShape = 0; iShape < mShapes.size(); iShape++)
		{
			if (mShapes.at(iShape).name == shapeName)
			{
				LoadShapeByIndex(iShape);
				return *this;
			}
		}

		assert(false && "Shape with specified name is not found");
		return *this;
	}

	ObjImporter& ObjImporter::LoadAllShapes()
	{
		assert(State::ObjLoaded == mState);

		for (size_t iShape = 0; iShape < mShapes.size(); iShape++)
		{
			LoadShapeByIndex(iShape);
		}

		return *this;
	}

	Mesh2 ObjImporter::Build()
	{
		assert(State::ObjLoaded == mState);
		mState = State::Committed;

		Mesh2 mesh = std::move(mImportedMesh);
		return mesh;
	}

	std::vector<VertexParams> ObjImporter::GetVertexParamsFromAttributes(
		MeshAttribute attributes
	)
	{
		std::vector<VertexParams> params
		{
			VertexParams{ 0, VertexFormat::Float3 }
		};

		if ((attributes & MeshAttribute::Normal) != MeshAttribute::None)
		{
			params.emplace_back(0, VertexFormat::Float3);
		}
		if ((attributes & MeshAttribute::UV) != MeshAttribute::None)
		{
			params.emplace_back(0, VertexFormat::Float2);
		}

		return params;
	}

	void ObjImporter::LoadShapeByIndex(size_t shapeIndex)
	{
		assert(!mShapeIsLoaded.at(shapeIndex) && "Shape is already loaded");
		mShapeIsLoaded.at(shapeIndex) = true;

		const tinyobj::shape_t& shapeData = mShapes.at(shapeIndex);
		const size_t triangleCount = shapeData.mesh.num_face_vertices.size();
		const size_t vertexCount = triangleCount * shapeData.mesh.num_face_vertices.front();

		std::vector<float> vertexData;
		MeshAttribute attributes = MeshAttribute::Position;

		for (size_t iVertex = 0; iVertex < vertexCount; iVertex++)
		{
			auto[iPos, iNormal, iTexCoord] = shapeData.mesh.indices.at(iVertex);

			assert(iPos >= 0 && "Expected position vertex attribute");

			tinyobj::real_t pos[]
			{
				mAttributes.vertices.at(3 * iPos + 0),
				mAttributes.vertices.at(3 * iPos + 1),
				mAttributes.vertices.at(3 * iPos + 2),
			};
			vertexData.insert(std::end(vertexData), std::begin(pos), std::end(pos));

			if (iNormal >= 0 && iNormal < mAttributes.normals.size())
			{
				tinyobj::real_t normal[]
				{
					mAttributes.normals.at(3 * iNormal + 0),
					mAttributes.normals.at(3 * iNormal + 1),
					mAttributes.normals.at(3 * iNormal + 2),
				};
				vertexData.insert(std::end(vertexData), std::begin(normal), std::end(normal));
				attributes |= MeshAttribute::Normal;
			}
			else if ((attributes & MeshAttribute::Normal) != MeshAttribute::None)
			{
				// TODO: add warning.
				vertexData.insert(std::end(vertexData), { 0.0f, 0.0f, 0.0f });
			}

			if (iTexCoord >= 0 && iTexCoord < mAttributes.texcoords.size())
			{
				tinyobj::real_t texcoord[]
				{
					mAttributes.texcoords.at(2 * iTexCoord + 0),
					mAttributes.texcoords.at(2 * iTexCoord + 1),
				};
				vertexData.insert(std::end(vertexData), std::begin(texcoord), std::end(texcoord));
				attributes |= MeshAttribute::UV;
			}
			else if ((attributes & MeshAttribute::UV) != MeshAttribute::None)
			{
				// TODO: add warning.
				vertexData.insert(std::end(vertexData), { 0.0f, 0.0f });
			}
		}

		const size_t vertexSize = CalculateVertexSize(attributes);

		Submesh shape(shapeData.name);
		shape.SetVertexBufferSize(vertexSize, vertexCount);
		shape.SetVertexBufferData(vertexData.data());
		shape.SetVertexParams(GetVertexParamsFromAttributes(attributes));

		mImportedMesh.Insert(std::move(shape));
	}
}

