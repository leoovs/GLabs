#pragma once

#include "glabs/graphics/ogl_shader_program.hpp"

namespace glabs
{
	class ShaderLibraryStorage
	{
	public:
		ShaderLibraryStorage() = default;
		ShaderLibraryStorage(ShaderStage stage);

		ShaderLibraryStorage(ShaderLibraryStorage&& other) = default;

		ShaderLibraryStorage& operator=(ShaderLibraryStorage&& other) = default;

		OglShaderProgram& FetchFromFile(
			std::string_view filename,
			const std::string& shaderName
		);
		OglShaderProgram& FromSource(
			std::string_view shaderSource,
			const std::string& shaderName
		);
		OglShaderProgram& FromName(
			const std::string& shaderName
		);

	private:
		OglShaderProgram& Emplace(OglShaderProgram program, const std::string& shaderName);

		ShaderStage mStage = ShaderStage::Vertex;
		std::unordered_map<
			std::string,
			OglShaderProgram
		> mShadersByName;
	};

	class ShaderLibrary
	{
	public:
		ShaderLibrary();

		ShaderLibraryStorage& operator[](ShaderStage stage);

	private:
		void InitStorages();

		std::array<ShaderLibraryStorage, size_t(ShaderStage::Count_)> mStoragesByStage;
	};
}

