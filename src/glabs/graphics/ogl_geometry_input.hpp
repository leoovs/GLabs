#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/ogl_buffers.hpp"

namespace glabs
{
	enum class VertexFormat
	{
		Float,
		Float2,
		Float3,
	};

	constexpr size_t GetVertexFormatByteWidth(VertexFormat format)
	{
		switch (format)
		{
		case VertexFormat::Float:
			return sizeof(float);
		case VertexFormat::Float2:
			return sizeof(float[2]);
		case VertexFormat::Float3:
			return sizeof(float[3]);
		}

		return 0;
	}

	constexpr size_t GetVertexFormatSize(VertexFormat format)
	{
		switch (format)
		{
		case VertexFormat::Float:
			return 1;
		case VertexFormat::Float2:
			return 2;
		case VertexFormat::Float3:
			return 3;
		}

		return 0;
	}

	constexpr GLenum GetVertexFormatNativeType(VertexFormat format)
	{
		// Currently, floats are the only supported format.
		return GL_FLOAT;
	}

	struct VertexParams
	{
		size_t InputSlot = 0;
		VertexFormat Format = VertexFormat::Float;
	};

	class OglGeometryInput
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglGeomtryInput (aka Vertex Array)";

			OglBuffers VertexBuffers;
			OglBuffer* IndexBuffer = nullptr;
			std::vector<VertexParams> Vertices;
		};

		OglGeometryInput() = default;
		OglGeometryInput(Params params);

		OglGeometryInput(OglGeometryInput&& other) noexcept;

		OglGeometryInput& operator=(OglGeometryInput&& other) noexcept;

		~OglGeometryInput();

		void BindToPipeline();

	private:
		void CreateNativeVertexArray();
		void DestroyNativeVertexArray();

		void AssignVertexBuffers();
		void AssignIndexBuffer();
		void AssignVertexFormats();

		Params mParams;
		GLuint mNativeVertexArray = 0;
	};
}

