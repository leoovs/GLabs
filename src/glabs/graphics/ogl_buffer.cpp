#include "glabs/graphics/ogl_buffer.hpp"

namespace glabs
{
	OglBuffer::OglBuffer(const Params& params)
		: mParams(params)
	{
		CreateNativeBuffer();
	}

	OglBuffer::OglBuffer(OglBuffer&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeBuffer(std::exchange(other.mNativeBuffer, 0))
	{}

	OglBuffer& OglBuffer::operator=(OglBuffer&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeBuffer();
		mParams = std::move(other.mParams);
		mNativeBuffer = std::exchange(other.mNativeBuffer, 0);

		return *this;
	}

	OglBuffer::~OglBuffer()
	{
		DestroyNativeBuffer();
	}

	const OglBuffer::Params& OglBuffer::GetParams() const
	{
		return mParams;
	}

	size_t OglBuffer::GetSize() const
	{
		return mParams.ElementSize * mParams.ElementCount;
	}

	GLuint OglBuffer::GetNativeBuffer() const
	{
		return mNativeBuffer;
	}

	void OglBuffer::SetData(const void* data)
	{
		glNamedBufferSubData(
			mNativeBuffer,
			0,
			GLsizeiptr(GetSize()),
			data
		);
	}

	void OglBuffer::CreateNativeBuffer()
	{
		glCreateBuffers(1, &mNativeBuffer);
		glNamedBufferStorage(
			mNativeBuffer,
			static_cast<GLsizeiptr>(GetSize()),
			nullptr,
			GL_DYNAMIC_STORAGE_BIT
		);

		glObjectLabel(
			GL_BUFFER,
			mNativeBuffer,
			GLsizei(mParams.DebugName.length()),
			mParams.DebugName.data()
		);
	}

	void OglBuffer::BindToPipeline()
	{
		glBindBuffer(mParams.Target, mNativeBuffer);
	}

	void OglBuffer::DestroyNativeBuffer()
	{
		if (0 != mNativeBuffer)
		{
			glDeleteBuffers(1, &mNativeBuffer);
			mNativeBuffer = 0;
		}
	}
}

