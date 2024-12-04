#pragma once

#include "glabs/pch.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"

namespace glabs
{
	class OglFramebuffer
	{
	public:
		enum class Attachment
		{
			Color0,
			Color1,
			Color2,
			DepthStencil,

			Count_,
		};

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

		void SetAttachment(
			Attachment attachmentName,
			OglTexture2D& texture,
			int32_t mipLevelIndex = 0,
			int32_t arrayIndex = 0
		);
		void RemoveAttachment(Attachment attachmentName);
		OglTexture2D& GetAttachment(Attachment attachmentName) const;

		void BindToPipeline();

		void ClearColor(glm::vec4 color, size_t index = 0);
		void ClearDepth(float depth);

	private:
		static GLenum AttachmentToNativeAttachment(Attachment attachment);

		void CreateNativeFramebuffer();
		void DestroyNativeFramebuffer();

		Params mParams;
		GLuint mNativeFramebuffer = 0;
		std::array<OglTexture2D*, size_t(Attachment::Count_)> mAttachments = {};
		std::array<int32_t, size_t(Attachment::Count_)> mAttachmentMipMapIndices = {};
		std::array<int32_t, size_t(Attachment::Count_)> mAttachmentArrayIndices = {};
	};
}

