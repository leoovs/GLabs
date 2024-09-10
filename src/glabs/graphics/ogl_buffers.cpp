#include "glabs/graphics/ogl_buffers.hpp"

namespace glabs
{
	OglBuffer*& OglBuffers::operator[](size_t slot)
	{
		return Emplace(slot);
	}

	OglBuffer* OglBuffers::operator[](size_t slot) const
	{
		return mBuffers.at(slot);
	}

	OglBuffer*& OglBuffers::Emplace(size_t slot)
	{
		if (slot >= mBuffers.size())
		{
			mBuffers.resize(slot + 1);
		}
		return mBuffers.at(slot);
	}

	OglBuffers::Iterator OglBuffers::begin() const
	{
		return mBuffers.cbegin();
	}

	OglBuffers::Iterator OglBuffers::end() const
	{
		return mBuffers.cend();
	}

	size_t OglBuffers::GetSlotFromIterator(Iterator it) const
	{
		return std::distance(mBuffers.begin(), it);
	}
}

