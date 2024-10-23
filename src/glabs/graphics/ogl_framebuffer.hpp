#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"

namespace glabs
{
	class OglFramebuffer
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed Framebuffer";

			OglTexture2D::Params ColorAttachmentParams{ "Unnamed Framebuffer color attachment" };
			OglTexture2D::Params DepthAttachmentParams{ "Unnamed Framebuffer depth attachment" };
			size_t ColorAttachmentMipMapIndex = 0;
			size_t DepthAttachmentMipMapIndex = 0;
		};

		static void BindDefaultToPipeline();

		OglFramebuffer();

		OglFramebuffer(Params params);

		OglFramebuffer(OglFramebuffer&& other) noexcept;

		OglFramebuffer& operator=(OglFramebuffer&& other) noexcept;

		~OglFramebuffer();

		const Params& GetParams() const;
		GLuint GetNativeFramebuffer() const;

		const OglTexture2D& GetColorAttachment() const;
		const OglTexture2D& GetDepthAttachment() const;

		void BindToPipeline();

		void ClearColor(glm::vec4 color);
		void ClearDepth(float depth);

	private:
		void QueryParamsFromDefaultNativeFramebuffer();

		void CreateNativeFramebuffer();
		void DestroyNativeFramebuffer();

		void AssignNativeFramebufferAttachments();

		Params mParams;

		GLuint mNativeFramebuffer = 0;
		OglTexture2D mColorAttachment;
		OglTexture2D mDepthAttachment;
	};
}

