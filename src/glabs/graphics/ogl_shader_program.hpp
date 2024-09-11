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
		default:
			return 0;
		}
	}

	constexpr GLbitfield GetNativeShaderStageBit(ShaderStage stage)
	{
		switch (stage)
		{
		case ShaderStage::Vertex:
			return GL_VERTEX_SHADER_BIT;
		case ShaderStage::Fragment:
			return GL_FRAGMENT_SHADER_BIT;
		default:
			return 0;
		}
	}

	class OglUniformLocationCache
	{
	public:
		static constexpr GLint cInvalidUniformLocation = -1;

		void SetNativeShaderProgram(GLuint nativeShaderProgram);

		GLint GetUniformLocation(const std::string& uniformName) const;

	private:
		GLuint mNativeShaderProgram = 0;
		mutable std::unordered_map<std::string, GLint> mCachedLocations;
	};

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

		void SetUniform(const std::string& name, float value);

		const Params& GetParams() const;
		GLuint GetNativeShaderProgram() const;

	private:
		void CreateNativeShaderProgram();
		void DestroyNativeShaderProgram();

		void DisplayBuildingStatus();

		Params mParams;
		GLuint mNativeShaderProgram;
		OglUniformLocationCache mUniformLocationCache;
	};
}

