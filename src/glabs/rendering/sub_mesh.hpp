#pragma once

#include "glabs/graphics/ogl_buffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"

namespace glabs
{
	class Submesh
	{
	public:
		Submesh(std::string_view name = "Unnamed Submesh");

		Submesh(Submesh&& other) noexcept = default;

		Submesh& operator=(Submesh&& other) noexcept = default;

		void SetName(std::string_view name);
		std::string_view GetName() const;

		void SetVertexBufferSize(size_t vertexSize, size_t vertexCount);
		void SetIndexBufferSize(size_t indicesCount);

		void SetVertexBufferData(const void* data);
		void SetIndexBufferData(const void* data);

		void SetVertexParams(std::vector<VertexParams> vertexParams);

		size_t GetVertexCount() const;
		size_t GetIndexCount() const;
		const OglGeometryInput& GetGeometry() const;

	private:
		std::string mName = "Unnamed Submesh";

		OglBuffer mVertexBuffer;
		OglBuffer mIndexBuffer;
		OglGeometryInput mGeometry;
	};
}

