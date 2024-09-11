#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	enum class ShaderStage
	{
		Vertex,
		Fragment,

		Count_,
	};

	constexpr GLenum GetNativeShaderType(ShaderStage stage)
	{
		switch (stage)
		{
		case ShaderStage::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderStage::Fragment:
			return GL_FRAGMENT_SHADER;
		}

		return 0;
	}

	constexpr GLbitfield GetNativeShaderStageBit(ShaderStage stage)
	{
		switch (stage)
		{
		case ShaderStage::Vertex:
			return GL_VERTEX_SHADER_BIT;
		case ShaderStage::Fragment:
			return GL_FRAGMENT_SHADER_BIT;
		}

		return 0;
	}

	class OglShaderProgram
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglShaderProgram";

			ShaderStage Stage = ShaderStage::Vertex;
			std::string Source;
		};

		OglShaderProgram(Params params);

		OglShaderProgram(OglShaderProgram&& other) noexcept;

		OglShaderProgram& operator=(OglShaderProgram&& other) noexcept;

		~OglShaderProgram();

		const Params& GetParams() const;
		GLuint GetNativeShaderProgram() const;

	private:
		void CreateNativeShaderProgram();
		void DestroyNativeShaderProgram();

		Params mParams;
		GLuint mNativeShaderProgram;
	};
}

