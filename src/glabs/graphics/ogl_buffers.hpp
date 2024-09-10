#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/ogl_buffer.hpp"

namespace glabs
{
	class OglBuffers
	{
	public:
		OglBuffer*& operator[](size_t slot);
		OglBuffer* operator[](size_t slot) const;

	private:
		OglBuffer*& Emplace(size_t slot);

		std::vector<OglBuffer*> mBuffers;
	};
}

