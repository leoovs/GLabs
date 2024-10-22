#include "glabs/rendering/sub_mesh.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"

namespace glabs
{
	Submesh::Submesh(std::string_view name)
		: mName(name)
	{}

	void Submesh::SetName(std::string_view name)
	{
		mName = name;
	}

	std::string_view Submesh::GetName() const
	{
		return mName;
	}

	void Submesh::SetVertexBufferSize(size_t vertexSize, size_t vertexCount)
	{
		OglBuffer::Params vertexBufferParams;
		vertexBufferParams.DebugName = mName + ";vbo";
		vertexBufferParams.Target = GL_ARRAY_BUFFER;
		vertexBufferParams.ElementSize = vertexSize;
		vertexBufferParams.ElementCount = vertexCount;

		mVertexBuffer = OglBuffer(std::move(vertexBufferParams));
	}

	void Submesh::SetIndexBufferSize(size_t indicesCount)
	{
		OglBuffer::Params indexBufferParams;
		indexBufferParams.DebugName = mName + ";ibo";
		indexBufferParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		indexBufferParams.ElementSize = sizeof(unsigned int);
		indexBufferParams.ElementCount = indicesCount;

		mIndexBuffer = OglBuffer(std::move(indexBufferParams));
	}

	void Submesh::SetVertexBufferData(const void* data)
	{
		mVertexBuffer.SetData(data);
	}

	void Submesh::SetIndexBufferData(const void* data)
	{
		mIndexBuffer.SetData(data);
	}

	void Submesh::SetVertexParams(std::vector<VertexParams> vertexParams)
	{
		OglGeometryInput::Params geometryParams;
		geometryParams.DebugName = mName + ";vao";
		geometryParams.IndexBuffer = &mIndexBuffer;
		geometryParams.VertexBuffers[0] = &mVertexBuffer;
		geometryParams.Vertices = std::move(vertexParams);

		mGeometry = OglGeometryInput(std::move(geometryParams));
	}

	size_t Submesh::GetVertexCount() const
	{
		return mVertexBuffer.GetParams().ElementCount;
	}

	size_t Submesh::GetIndexCount() const
	{
		return mIndexBuffer.GetParams().ElementCount;
	}

	const OglGeometryInput& Submesh::GetGeometry() const
	{
		return mGeometry;
	}
}

