#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class OglGeometryInput
	{
	public:
		OglGeometryInput();

		OglGeometryInput(OglGeometryInput&& other) noexcept;

		OglGeometryInput& operator=(OglGeometryInput&& other) noexcept;

		~OglGeometryInput();

	private:
		void CreateNativeVertexArray();
		void DestroyNativeVertexArray();

		GLuint mNativeVertexArray = 0;
	};
}

