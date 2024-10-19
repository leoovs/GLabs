#include "glabs/rendering/mesh.hpp"

namespace glabs
{
	size_t Mesh::CalculateVertexByteWidth(Attributes attribFlags)
	{
		size_t size = 0;

		if (Attributes::None != (attribFlags & Attributes::Position))
		{
			size += sizeof(float[3]);
		}
		if (Attributes::None != (attribFlags & Attributes::Normal))
		{
			size += sizeof(float[3]);
		}
		if (Attributes::None != (attribFlags & Attributes::UV))
		{
			size += sizeof(float[2]);
		}

		return size;
	}

	Mesh Mesh::MakeCube(float side)
	{
		// TODO: implement cube mesh generation.
		return Mesh();
	}

	Mesh Mesh::FromObjFile(const std::filesystem::path& meshFile)
	{
		if (!std::filesystem::exists(meshFile))
		{
			return MakeCube(0.5f);
		}

		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string errors;

		bool loadSuccessful = tinyobj::LoadObj(&attributes, &shapes, &materials, &errors, meshFile.string().c_str());
		if (!loadSuccessful)
		{
			std::cerr << errors << '\n';
			return MakeCube(0.5f);
		}

		// Assume we have only one shape per .obj file.

		tinyobj::shape_t& shape = shapes.front();
		size_t faceCount = shape.mesh.num_face_vertices.size();

		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		Attributes attribFlags = Attributes::Position;

		for (size_t iFace = 0; iFace < faceCount; iFace++)
		{
			for (size_t iVertex = 0; iVertex < 3; iVertex++)
			{
				indices.push_back(indices.size());
				tinyobj::index_t& index = shape.mesh.indices.at(3 * iFace + iVertex);

				tinyobj::real_t vx = attributes.vertices.at(3 * index.vertex_index + 0);
				tinyobj::real_t vy = attributes.vertices.at(3 * index.vertex_index + 1);
				tinyobj::real_t vz = attributes.vertices.at(3 * index.vertex_index + 2);

				vertices.push_back(vx);
				vertices.push_back(vy);
				vertices.push_back(vz);

				if (index.normal_index >= 0)
				{
					tinyobj::real_t nx = attributes.normals.at(3 * index.normal_index + 0);
					tinyobj::real_t ny = attributes.normals.at(3 * index.normal_index + 1);
					tinyobj::real_t nz = attributes.normals.at(3 * index.normal_index + 2);

					vertices.push_back(nx);
					vertices.push_back(ny);
					vertices.push_back(nz);
					attribFlags |= Attributes::Normal;
				}

				if (index.texcoord_index >= 0)
				{
					tinyobj::real_t tx = attributes.texcoords.at(2 * size_t(index.texcoord_index) + 0);
					tinyobj::real_t ty = attributes.texcoords.at(2 * size_t(index.texcoord_index) + 1);

					vertices.push_back(tx);
					vertices.push_back(ty);
					attribFlags |= Attributes::UV;
				}
			}
		}

		size_t vertexSize = CalculateVertexByteWidth(attribFlags);

		OglBuffer vertexBuffer({
				shape.name + ";vertices",
				GL_ARRAY_BUFFER,
				vertexSize,
				vertices.size() / vertexSize
		});
		vertexBuffer.SetData(vertices.data());

		OglBuffer indexBuffer({
				shape.name + ";indices",
				GL_ELEMENT_ARRAY_BUFFER,
				sizeof(indices.front()),
				indices.size()
		});
		indexBuffer.SetData(indices.data());

		return Mesh(std::move(vertexBuffer), std::move(indexBuffer), attribFlags, shape.name);
	}

	Mesh::Mesh(
			OglBuffer vertices,
			OglBuffer indices,
			Attributes attribs,
			std::string name
	)
		: mVertices(std::move(vertices))
		, mIndices(std::move(indices))
		, mAttribs(attribs)
		, mName(std::move(name))
	{
		CreateGeometryFromAttribs();
	}

	std::string_view Mesh::GetName() const
	{
		return mName;
	}

	size_t Mesh::GetIndexCount() const
	{
		return mIndices.GetParams().ElementCount;
	}

	size_t Mesh::GetVertexCount() const
	{
		return mVertices.GetParams().ElementCount;
	}

	void Mesh::SetName(std::string name)
	{
		mName = std::move(name);
	}

	void Mesh::BindToPipeline() const
	{
		mGeometry.BindToPipeline();
	}

	void Mesh::CreateGeometryFromAttribs()
	{
		OglGeometryInput::Params geometryParams;
		geometryParams.DebugName = mName;
		geometryParams.VertexBuffers[0] = &mVertices;
		geometryParams.IndexBuffer = &mIndices;

		if ((mAttribs & Attributes::Position) != Attributes::None)
		{
			geometryParams.Vertices.emplace_back(size_t(0), VertexFormat::Float3);
		}
		if ((mAttribs & Attributes::Normal) != Attributes::None)
		{
			geometryParams.Vertices.emplace_back(size_t(0), VertexFormat::Float3);
		}
		if ((mAttribs & Attributes::UV) != Attributes::None)
		{
			geometryParams.Vertices.emplace_back(size_t(0), VertexFormat::Float2);
		}

		mGeometry = OglGeometryInput(std::move(geometryParams));
	}
}

