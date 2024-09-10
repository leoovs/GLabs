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

	constexpr size_t GetVertexFormatSize(VertexFormat format)
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
			OglBuffers VertexBuffers;
			OglBuffer* IndexBuffer = nullptr;
			std::vector<VertexParams> Vertices;
		};

		OglGeometryInput(Params params);

		OglGeometryInput(OglGeometryInput&& other) noexcept;

		OglGeometryInput& operator=(OglGeometryInput&& other) noexcept;

		~OglGeometryInput();

	private:
		void CreateNativeVertexArray();
		void DestroyNativeVertexArray();

		Params mParams;
		GLuint mNativeVertexArray = 0;
	};
}

