#include "glabs/graphics/ogl_framebuffer.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace glabs
{
	OglFramebuffer::OglFramebuffer(Params params)
		: mParams(std::move(params))
	{
		CreateNativeFramebuffer();
	}

	OglFramebuffer::OglFramebuffer(OglFramebuffer&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeFramebuffer(std::exchange(other.mNativeFramebuffer, 0))
		, mAttachments(std::exchange(other.mAttachments, {}))
	{}

	OglFramebuffer& OglFramebuffer::operator=(OglFramebuffer&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeFramebuffer();
		mParams = std::move(other.mParams);
		mNativeFramebuffer = std::exchange(other.mNativeFramebuffer, 0);
		mAttachments = std::exchange(other.mAttachments, {});

		return *this;
	}

	OglFramebuffer::~OglFramebuffer()
	{
		DestroyNativeFramebuffer();
	}

	const OglFramebuffer::Params& OglFramebuffer::GetParams() const
	{
		return mParams;
	}

	GLuint OglFramebuffer::GetNativeFramebuffer() const
	{
		return mNativeFramebuffer;
	}

	void OglFramebuffer::SetAttachment(
		Attachment attachmentName,
		OglTexture2D& texture,
		size_t mipLevelIndex
	)
	{
		assert(0 != mNativeFramebuffer);

		auto iAttachment = static_cast<size_t>(attachmentName);

		// TODO: replace with glNamedFramebufferTextureLayer to support cube maps.
		glNamedFramebufferTexture(
			mNativeFramebuffer,
			AttachmentToNativeAttachment(attachmentName),
			(mAttachments.at(iAttachment) = &texture)->GetNativeTexture2D(),
			mAttachmentMipMapIndices.at(iAttachment) = mipLevelIndex
		);
	}

	void OglFramebuffer::RemoveAttachment(Attachment attachmentName)
	{
		assert(0 != mNativeFramebuffer);

		auto iAttachment = static_cast<size_t>(attachmentName);

		mAttachments.at(iAttachment) = nullptr;
		mAttachmentMipMapIndices.at(iAttachment) = 0;

		glNamedFramebufferTexture(
			mNativeFramebuffer,
			AttachmentToNativeAttachment(attachmentName),
			0,
			0
		);
	}

	OglTexture2D& OglFramebuffer::GetAttachment(Attachment attachmentName) const
	{
		assert(0 != mNativeFramebuffer);

		auto iAttachment = static_cast<size_t>(attachmentName);
		OglTexture2D* attachment = mAttachments.at(iAttachment);

		assert(nullptr != attachment);

		return *attachment;
	}

	void OglFramebuffer::BindToPipeline()
	{
		assert(0 != mNativeFramebuffer);
		// TODO: should we explicitly provide target instead of hardcoding it?
		glBindFramebuffer(GL_FRAMEBUFFER, mNativeFramebuffer);
	}

	void OglFramebuffer::ClearColor(glm::vec4 color)
	{
		assert(0 != mNativeFramebuffer);
		glClearNamedFramebufferfv(mNativeFramebuffer, GL_COLOR, 0, glm::value_ptr(color));
	}

	void OglFramebuffer::ClearDepth(float depth)
	{
		assert(0 != mNativeFramebuffer);
		glClearNamedFramebufferfv(mNativeFramebuffer, GL_DEPTH, 0, &depth);
	}

	GLenum OglFramebuffer::AttachmentToNativeAttachment(Attachment attachment)
	{
		switch (attachment)
		{
		case Attachment::Color0:
			return GL_COLOR_ATTACHMENT0;
		case Attachment::Color1:
			return GL_COLOR_ATTACHMENT1;
		case Attachment::Color2:
			return GL_COLOR_ATTACHMENT2;
		case Attachment::DepthStencil:
			return GL_DEPTH_STENCIL_ATTACHMENT;
		default:
			return GL_FALSE;
		}
	}

	void OglFramebuffer::CreateNativeFramebuffer()
	{
		glCreateFramebuffers(1, &mNativeFramebuffer);

		glObjectLabel(
			GL_FRAMEBUFFER,
			mNativeFramebuffer,
			GLsizei(mParams.DebugName.length()),
			mParams.DebugName.data()
		);
	}

	void OglFramebuffer::DestroyNativeFramebuffer()
	{
		glDeleteFramebuffers(1, &mNativeFramebuffer);
		mNativeFramebuffer = 0;
	}
}

