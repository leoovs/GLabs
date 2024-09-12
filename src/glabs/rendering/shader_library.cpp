#include "glabs/rendering/shader_library.hpp"

namespace glabs
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// ShaderLibraryStorage
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	ShaderLibraryStorage::ShaderLibraryStorage(ShaderStage stage)
		: mStage(stage)
	{}

	OglShaderProgram& ShaderLibraryStorage::FetchFromFile(
		std::string_view filename,
		const std::string& shaderName
	)
	{
		std::string shaderSource;
		std::string lineBuffer;

		std::ifstream shaderSourceFile(filename.data());
		while (std::getline(shaderSourceFile, lineBuffer))
		{
			shaderSource += lineBuffer + '\n';
		}

		return FromSource(shaderSource, shaderName);
	}

	OglShaderProgram& ShaderLibraryStorage::FromSource(
		std::string_view shaderSource,
		const std::string& shaderName
	)
	{
		OglShaderProgram::Params shaderParams;
		shaderParams.DebugName = shaderName;
		shaderParams.Stage = mStage;
		shaderParams.Source = shaderSource;

		return Emplace(OglShaderProgram(std::move(shaderParams)), shaderName);
	}

	OglShaderProgram& ShaderLibraryStorage::FromName(
		const std::string& shaderName
	)
	{
		return mShadersByName.at(shaderName);
	}

	OglShaderProgram& ShaderLibraryStorage::Emplace(
		OglShaderProgram program,
		const std::string& shaderName
	)
	{
		return mShadersByName[shaderName] = std::move(program);
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// ShaderLibrary
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	ShaderLibrary::ShaderLibrary()
	{
		InitStorages();
	}

	ShaderLibraryStorage& ShaderLibrary::operator[](ShaderStage stage)
	{
		return mStoragesByStage.at(size_t(stage));
	}

	void ShaderLibrary::InitStorages()
	{
		ShaderStage supportedStages[]{ ShaderStage::Vertex, ShaderStage::Fragment };

		for (ShaderStage stage : supportedStages)
		{
			mStoragesByStage.at(size_t(stage)) = ShaderLibraryStorage(stage);
		}
	}
}

