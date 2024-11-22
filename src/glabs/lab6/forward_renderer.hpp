#pragma once

#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/pch.hpp"
#include "glabs/rendering/camera.hpp"
#include "glabs/rendering/mesh.hpp"

namespace glabs
{
	struct DirectionalLight
	{
		glm::vec3 Direction = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 Ambient = glm::vec3(1.0f);
		glm::vec3 Diffuse = glm::vec3(1.0f);
		glm::vec3 Specular = glm::vec3(1.0f);
	};

	struct PointLight
	{
		std::reference_wrapper<int> Cast;
		std::reference_wrapper<glm::vec3> Position;
		std::reference_wrapper<float> Constant;
		std::reference_wrapper<float> Linear;
		std::reference_wrapper<float> Quadratic;
		std::reference_wrapper<glm::vec3> Ambient;
		std::reference_wrapper<glm::vec3> Diffuse;
		std::reference_wrapper<glm::vec3> Specular;
	};

	class ForwardRenderer
	{
	public:
		enum class ShaderPipeline
		{
			LightBoxPass,
			MeshPass,

			Count_,
		};

		struct Material
		{
			glm::vec3 Ambient = glm::vec3(0.0f);
			glm::vec3 Diffuse = glm::vec3(1.0f);
			glm::vec3 Specular = glm::vec3(1.0f);
			float Shininess = 0.5f;
		};

		struct Params
		{
			int32_t LightCount = 8;
		};

		ForwardRenderer(Params params);

		PointLight GetPointLight(size_t lightIndex);
		size_t GetPointLightCount() const;
		OglProgramPipeline& GetPipeline(ShaderPipeline pipelineKind);
		const DirectionalLight& GetDirectionalLight() const;

		void SetCamera(const Camera& camera);
		void SetProjection(const glm::mat4& projection);
		void SetDirectionalLight(const DirectionalLight& dirLight);

		void BeginMeshPass();
		void RenderMeshWithMaterial(
			const Mesh& mesh,
			const Material& material,
			const glm::mat4& model
		);
		void EndMeshPass();

		void BeginLightBoxPass();
		void RenderLightBoxes();
		void EndLightBoxPass();

	private:
		void AllocateLights();

		void SetMeshPassMaterialData(const Material& material, const glm::mat4& model);
		void SetMeshPassGlobalData();

		void SetLightBoxPassUniforms();

		Params mParams;
		DirectionalLight mDirectionalLight;

		const Camera* mBoundCamera = nullptr;
		glm::mat4 mProjection = glm::mat4(1.0f);

		std::vector<int> mLightCasts;
		std::vector<glm::vec3> mLightPositions;
		std::vector<float> mLightConstant;
		std::vector<float> mLightLinear;
		std::vector<float> mLightQuadratic;
		std::vector<glm::vec3> mLightAmbient;
		std::vector<glm::vec3> mLightDiffuse;
		std::vector<glm::vec3> mLightSpecular;

		std::array<OglProgramPipeline, size_t(ShaderPipeline::Count_)> mShaders
		{
			OglProgramPipeline{{ "Forward - LightBoxPass" }},
			OglProgramPipeline{{ "Forward - MeshPass" }},
		};
	};
}

