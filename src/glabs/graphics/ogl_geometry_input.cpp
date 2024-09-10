#include "glabs/graphics/ogl_geometry_input.hpp"

namespace glabs
{
	OglGeometryInput::OglGeometryInput(Params params)
		: mParams(std::move(params))
	{
		CreateNativeVertexArray();
	}

	OglGeometryInput::OglGeometryInput(OglGeometryInput&& other) noexcept
		: mNativeVertexArray(std::exchange(other.mNativeVertexArray, 0))
	{}

	OglGeometryInput& OglGeometryInput::operator=(OglGeometryInput&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeVertexArray();
		mNativeVertexArray = std::exchange(other.mNativeVertexArray, 0);

		return *this;
	}

	OglGeometryInput::~OglGeometryInput()
	{
		DestroyNativeVertexArray();
	}

	void OglGeometryInput::CreateNativeVertexArray()
	{
		glCreateVertexArrays(1, &mNativeVertexArray);
	}

	void OglGeometryInput::DestroyNativeVertexArray()
	{
		glDeleteVertexArrays(1, &mNativeVertexArray);
		mNativeVertexArray = 0;
	}
}

