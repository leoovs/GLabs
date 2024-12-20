#pragma once

#include "glabs/graphics/ogl_framebuffer.hpp"
#include "glabs/graphics/ogl_geometry_input.hpp"
#include "glabs/graphics/ogl_program_pipeline.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"
#include "glabs/rendering/camera.hpp"
#include "glabs/rendering/sub_mesh.hpp"
#include "glabs/rendering/material.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	class ForwardRenderer
	{
	public:
		ForwardRenderer();

		void ResizeFrame(const glm::ivec2& frameSize);

		void SetCamera(const Camera& camera);
		void SetProjection(const glm::mat4& projection);

		OglProgramPipeline& GetMainPassShaders();
		OglProgramPipeline& GetScreenPassShaders();

		void BeginMainPass();
		void DrawMesh(const Submesh& mesh, const Material& material, const glm::mat4& model);
		void EndMainPass();

		void BeginScreenPass();
		void DrawToScreen();
		void EndScreenPass();

	private:
		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;

		void SetupRenderBuffers(const glm::ivec2& frameSize);

		const Camera* mCameraView = nullptr;
		const glm::mat4* mProjectionView = nullptr;

		glm::ivec2 mFrameSize = { 1280, 720 };
		OglFramebuffer mOutput = OglFramebuffer({ "Fwd renderer output" });
		OglTexture2D mColorBuffer;
		OglTexture2D mDepthBuffer;

		OglProgramPipeline mMainPassShaders{{ "Main pass shaders" }};
		OglProgramPipeline mScreenPassShaders{{ "Screen pass shaders" }};

		OglGeometryInput mEmptyGeometryInput = OglGeometryInput({ "Empty geometry input" });
	};
}

