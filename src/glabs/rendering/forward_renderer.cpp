#include "glabs/rendering/forward_renderer.hpp"

namespace glabs
{
	ForwardRenderer::ForwardRenderer()
	{
		SetupRenderBuffers(mFrameSize);
	}

	void ForwardRenderer::ResizeFrame(const glm::ivec2& frameSize)
	{
		SetupRenderBuffers(mFrameSize = frameSize);
	}

	void ForwardRenderer::SetCamera(const Camera& camera)
	{
		mCameraView = &camera;
	}

	void ForwardRenderer::SetProjection(const glm::mat4& projection)
	{
		mProjectionView = &projection;
	}

	OglProgramPipeline& ForwardRenderer::GetMainPassShaders()
	{
		return mMainPassShaders;
	}

	OglProgramPipeline& ForwardRenderer::GetScreenPassShaders()
	{
		return mScreenPassShaders;
	}

	void ForwardRenderer::BeginMainPass()
	{
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, mFrameSize.x, mFrameSize.y);

		mOutput.ClearColor(glm::vec4(0.5f));
		mOutput.ClearDepth(1.0f);

		mOutput.BindToPipeline();
		mMainPassShaders.BindToPipeline();

		auto& vs = mMainPassShaders[ShaderStage::Vertex].Get();
		vs.SetUniform("uViewProj", GetProjectionMatrix() * GetViewMatrix());
	}

	void ForwardRenderer::DrawMesh(const Submesh& mesh, const Material& material, const glm::mat4& model)
	{
		auto& vs = mMainPassShaders[ShaderStage::Vertex].Get();
		vs.SetUniform("uModel", model);
		vs.SetUniform("uITModel", glm::transpose(glm::inverse(model)));

		material[MaterialKind::Albedo].BindToPipeline(0);
		material[MaterialKind::Normal].BindToPipeline(1);
		material[MaterialKind::Specular].BindToPipeline(2);

		mesh.GetGeometry().BindToPipeline();
		glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertexCount());
	}

	void ForwardRenderer::EndMainPass()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void ForwardRenderer::BeginScreenPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, mFrameSize.x, mFrameSize.y);
		glClear(GL_COLOR_BUFFER_BIT);

		mScreenPassShaders.BindToPipeline();
		mColorBuffer.BindToPipeline(0);
		mEmptyGeometryInput.BindToPipeline();
	}

	void ForwardRenderer::DrawToScreen()
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void ForwardRenderer::EndScreenPass()
	{
	}

	glm::mat4 ForwardRenderer::GetViewMatrix() const
	{
		return mCameraView
			? mCameraView->CalculateLookAt()
			: glm::mat4(1.0f);
	}

	glm::mat4 ForwardRenderer::GetProjectionMatrix() const
	{
		return mProjectionView
			? *mProjectionView
			: glm::mat4(1.0f);
	}

	void ForwardRenderer::SetupRenderBuffers(const glm::ivec2& frameSize)
	{
		OglTexture2D::Params renderBufferParams;
		renderBufferParams.DebugName = "Fwd renderer color buffer";
		renderBufferParams.Width = frameSize.x;
		renderBufferParams.Height = frameSize.y;
		renderBufferParams.Format = GraphicsFormat::R8G8B8_UNORM;

		mColorBuffer = OglTexture2D(renderBufferParams);

		renderBufferParams.Format = GraphicsFormat::D32_UNORM;
		renderBufferParams.DebugName = "Fwd renderer depth buffer";

		mDepthBuffer = OglTexture2D(std::move(renderBufferParams));

		mOutput.AttachTexture2D(FramebufferAttachment::Color0, mColorBuffer);
		mOutput.BindAttachment(FramebufferAttachment::Color0);

		mOutput.AttachTexture2D(FramebufferAttachment::Depth, mDepthBuffer);
		mOutput.BindAttachment(FramebufferAttachment::Depth);
	}
}

