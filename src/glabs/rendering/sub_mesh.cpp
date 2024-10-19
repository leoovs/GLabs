#include "glabs/rendering/sub_mesh.hpp"

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
		indexBufferParams.Target = GL_ELEMENT_ARRAY_BUFFER;
		indexBufferParams.DebugName = mName + ";ibo";
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

	void Submesh::SetLayoutParams(OglGeometryInput::Params layoutParams)
	{
		mLayout = OglGeometryInput(std::move(layoutParams));
	}

	const OglGeometryInput& Submesh::GetLayout() const
	{
		return mLayout;
	}
}

