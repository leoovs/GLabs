#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/ogl_buffer.hpp"

namespace glabs
{
	class OglBuffers
	{
	private:
		using Container = std::vector<OglBuffer*>;

	public:
		using Iterator = Container::const_iterator;

		OglBuffer*& operator[](size_t slot);
		OglBuffer* operator[](size_t slot) const;

		Iterator begin() const;
		Iterator end() const;

		size_t GetSlotFromIterator(Iterator it) const;

	private:
		OglBuffer*& Emplace(size_t slot);

		Container mBuffers;
	};
}

