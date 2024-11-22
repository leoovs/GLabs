#include "glabs/lab6/forward_renderer.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"

namespace glabs
{
	ForwardRenderer::ForwardRenderer(Params params)
		: mParams(std::move(params))
	{
		AllocateLights();
	}

	PointLight ForwardRenderer::GetPointLight(size_t lightIndex)
	{
		return PointLight
		{
			mLightCasts.at(lightIndex),
			mLightPositions.at(lightIndex),
			mLightConstant.at(lightIndex),
			mLightLinear.at(lightIndex),
			mLightQuadratic.at(lightIndex),
			mLightAmbient.at(lightIndex),
			mLightDiffuse.at(lightIndex),
			mLightSpecular.at(lightIndex),
		};
	}

	size_t ForwardRenderer::GetPointLightCount() const
	{
		return mParams.LightCount;
	}

	OglProgramPipeline& ForwardRenderer::GetPipeline(ShaderPipeline pipelineKind)
	{
		return mShaders.at(size_t(pipelineKind));
	}

	const DirectionalLight& ForwardRenderer::GetDirectionalLight() const
	{
		return mDirectionalLight;
	}

	void ForwardRenderer::SetCamera(const Camera& camera)
	{
		mBoundCamera = &camera;
	}

	void ForwardRenderer::SetProjection(const glm::mat4& projection)
	{
		mProjection = projection;
	}

	void ForwardRenderer::SetDirectionalLight(const DirectionalLight& dirLight)
	{
		mDirectionalLight = dirLight;
	}

	void ForwardRenderer::BeginMeshPass()
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		OglProgramPipeline& meshShaders = GetPipeline(ShaderPipeline::MeshPass);
		meshShaders.BindToPipeline();
		SetMeshPassGlobalData();
	}

	void ForwardRenderer::RenderMeshWithMaterial(
		const Mesh& mesh,
		const Material& material,
		const glm::mat4& model
	)
	{
		SetMeshPassMaterialData(material, model);
		mesh.ForEachShape([this](const Submesh& shape)
			{
				shape.GetGeometry().BindToPipeline();
				glDrawArrays(GL_TRIANGLES, 0, shape.GetVertexCount());
			}
		);
	}

	void ForwardRenderer::EndMeshPass() {}

	void ForwardRenderer::BeginLightBoxPass()
	{
		OglProgramPipeline& lightBoxShaders = GetPipeline(ShaderPipeline::LightBoxPass);
		lightBoxShaders.BindToPipeline();

		SetLightBoxPassUniforms();
	}

	void ForwardRenderer::RenderLightBoxes()
	{
		glDrawArrays(GL_TRIANGLES, 0, 36 * mParams.LightCount);
	}

	void ForwardRenderer::EndLightBoxPass() {}

	void ForwardRenderer::AllocateLights()
	{
		mLightCasts.resize(mParams.LightCount, 0);
		mLightPositions.resize(mParams.LightCount, glm::vec3(0.0f));
		mLightConstant.resize(mParams.LightCount, 1.0f);
		mLightLinear.resize(mParams.LightCount, 1.0f);
		mLightQuadratic.resize(mParams.LightCount, 1.0f);
		mLightAmbient.resize(mParams.LightCount, glm::vec3(1.0f));
		mLightDiffuse.resize(mParams.LightCount, glm::vec3(1.0f));
		mLightSpecular.resize(mParams.LightCount, glm::vec3(1.0f));
	}

	void ForwardRenderer::SetMeshPassMaterialData(const Material& material, const glm::mat4& model)
	{
		OglProgramPipeline& meshPass = mShaders.at(size_t(ShaderPipeline::MeshPass));
		OglShaderProgram& vs = meshPass[ShaderStage::Vertex].Get();

		vs.SetUniform("uM", model);
		vs.SetUniform("uITM", glm::transpose(glm::inverse(model)));

		OglShaderProgram& fs = meshPass[ShaderStage::Fragment].Get();

		fs.SetUniform("uMaterial.ambient", material.Ambient);
		fs.SetUniform("uMaterial.diffuse", material.Diffuse);
		fs.SetUniform("uMaterial.specular", material.Specular);
		fs.SetUniform("uMaterial.shininess", material.Shininess);
	}

	void ForwardRenderer::SetMeshPassGlobalData()
	{
		OglProgramPipeline& meshPass = GetPipeline(ShaderPipeline::MeshPass);
		OglShaderProgram& vs = meshPass[ShaderStage::Vertex].Get();

		glm::mat4 view = mBoundCamera->CalculateLookAt();
		vs.SetUniform("uVP", mProjection * view);

		OglShaderProgram& fs = meshPass[ShaderStage::Fragment].Get();

		fs.SetUniform("uViewPosition", mBoundCamera->GetEyePosition());

		fs.SetUniform("uDirectionalLight.direction", mDirectionalLight.Direction);
		fs.SetUniform("uDirectionalLight.ambient", mDirectionalLight.Ambient);
		fs.SetUniform("uDirectionalLight.diffuse", mDirectionalLight.Diffuse);
		fs.SetUniform("uDirectionalLight.specular", mDirectionalLight.Specular);

		fs.SetUniform("uPointLight.casts", mLightCasts);
		fs.SetUniform("uPointLight.positions", mLightPositions);
		fs.SetUniform("uPointLight.constants", mLightConstant);
		fs.SetUniform("uPointLight.linears", mLightLinear);
		fs.SetUniform("uPointLight.quadratics", mLightQuadratic);
		fs.SetUniform("uPointLight.ambient", mLightAmbient);
		fs.SetUniform("uPointLight.diffuse", mLightDiffuse);
		fs.SetUniform("uPointLight.specular", mLightSpecular);
	}

	void ForwardRenderer::SetLightBoxPassUniforms()
	{
		OglProgramPipeline& lightBoxPass = GetPipeline(ShaderPipeline::LightBoxPass);

		OglShaderProgram& vs = lightBoxPass[ShaderStage::Vertex].Get();

		glm::mat4 view = mBoundCamera->CalculateLookAt();
		vs.SetUniform("uVP", mProjection * view);
		vs.SetUniform("uLightPositions", mLightPositions);

		OglShaderProgram& ps = lightBoxPass[ShaderStage::Fragment].Get();
		ps.SetUniform("uPointLight.diffuse", mLightDiffuse);
		ps.SetUniform("uPointLight.casts", mLightCasts);
	}
}

