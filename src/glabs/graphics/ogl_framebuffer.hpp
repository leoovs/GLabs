#pragma once

#include "glabs/graphics/framebuffer_attachment.hpp"
#include "glabs/pch.hpp"
#include "glabs/graphics/ogl_framebuffer_binding.hpp"

namespace glabs
{
	class OglFramebuffer
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglFramebuffer";
		};

		static constexpr size_t cColorAttachmentCount = 8;
		static constexpr size_t cDepthStencilAttachmentCount = 1;

		OglFramebuffer() = default;

		OglFramebuffer(Params params);

		OglFramebuffer(OglFramebuffer&& other) noexcept;

		OglFramebuffer& operator=(OglFramebuffer&& other) noexcept;

		~OglFramebuffer();

		const Params& GetParams() const;
		GLuint GetNativeFramebuffer() const;
		void BindToPipeline();

		void ClearColor(glm::vec4 color, size_t index = 0);
		void ClearDepth(float depth);

		void AttachTexture2D(
			FramebufferAttachment attachment,
			const OglTexture2D& texture,
			int32_t arrayIndex = 0,
			int32_t mipLevelIndex = 0
		);

		void AttachCubemapFace(
			FramebufferAttachment attachment,
			const OglCubemap& cubemap,
			CubemapFace face,
			int32_t arrayIndex = 0,
			int32_t mipLevelIndex = 0
		);

		void BindAttachment(FramebufferAttachment attachment);
		void UnbindAttachment(FramebufferAttachment attachment);

	private:
		void CreateNativeFramebuffer();
		void DestroyNativeFramebuffer();

		Params mParams;
		GLuint mNativeFramebuffer = 0;
		std::array<
			std::unique_ptr<OglFramebufferBinding>,
			size_t(FramebufferAttachment::Count_)
		> mAttachments;
	};
}

