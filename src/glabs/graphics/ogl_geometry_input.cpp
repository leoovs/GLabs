#include "glabs/graphics/ogl_geometry_input.hpp"

namespace glabs
{
	OglGeometryInput::OglGeometryInput(Params params)
		: mParams(std::move(params))
	{
		CreateNativeVertexArray();
		AssignVertexBuffers();
		AssignIndexBuffer();
		AssignVertexFormats();
	}

	OglGeometryInput::OglGeometryInput(OglGeometryInput&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeVertexArray(std::exchange(other.mNativeVertexArray, 0))
	{}

	OglGeometryInput& OglGeometryInput::operator=(OglGeometryInput&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeVertexArray();
		mParams = std::move(other.mParams);
		mNativeVertexArray = std::exchange(other.mNativeVertexArray, 0);

		return *this;
	}

	OglGeometryInput::~OglGeometryInput()
	{
		DestroyNativeVertexArray();
	}

	void OglGeometryInput::BindToPipeline()
	{
		glBindVertexArray(mNativeVertexArray);
	}

	void OglGeometryInput::CreateNativeVertexArray()
	{
		glCreateVertexArrays(1, &mNativeVertexArray);
		glObjectLabel(
			GL_VERTEX_ARRAY,
			mNativeVertexArray,
			GLsizei(mParams.DebugName.length()),
			mParams.DebugName.data()
		);
	}

	void OglGeometryInput::DestroyNativeVertexArray()
	{
		if (0 != mNativeVertexArray)
		{
			glDeleteVertexArrays(1, &mNativeVertexArray);
			mNativeVertexArray = 0;
		}
	}

	void OglGeometryInput::AssignVertexBuffers()
	{
		for (auto it = mParams.VertexBuffers.begin();
			it != mParams.VertexBuffers.end();
			it++)
		{
			if (nullptr == *it)
			{
				continue;
			}

			size_t slot = mParams.VertexBuffers.GetSlotFromIterator(it);
			const OglBuffer* vertexBuffer = *it;
			assert(GL_ARRAY_BUFFER == vertexBuffer->GetParams().Target);

			glVertexArrayVertexBuffer(
				mNativeVertexArray,
				GLuint(slot),
				vertexBuffer->GetNativeBuffer(),
				0,
				GLsizei(vertexBuffer->GetParams().ElementSize)
			);
		}
	}

	void OglGeometryInput::AssignIndexBuffer()
	{
		if (nullptr == mParams.IndexBuffer)
		{
			return;
		}

		assert(GL_ELEMENT_ARRAY_BUFFER == mParams.IndexBuffer->GetParams().Target);
		glVertexArrayElementBuffer(mNativeVertexArray, mParams.IndexBuffer->GetNativeBuffer());
	}

	void OglGeometryInput::AssignVertexFormats()
	{
		std::vector<GLuint> relativeOffsetPerSlot(mParams.VertexBuffers.Size(), 0);

		for (auto it = mParams.Vertices.cbegin();
			it != mParams.Vertices.cend();
			it++)
		{
			GLuint attribIndex(std::distance(mParams.Vertices.cbegin(), it));

			glVertexArrayAttribFormat(
				mNativeVertexArray,
				attribIndex,
				GLint(GetVertexFormatSize(it->Format)),
				GetVertexFormatNativeType(it->Format),
				GL_FALSE,
				relativeOffsetPerSlot.at(it->InputSlot)
			);
			glVertexArrayAttribBinding(mNativeVertexArray, attribIndex, GLuint(it->InputSlot));
			glEnableVertexArrayAttrib(mNativeVertexArray, attribIndex);

			GLuint byteOffset = GetVertexFormatByteWidth(it->Format);
			relativeOffsetPerSlot.at(it->InputSlot) += byteOffset;
		}
	}
}

