#include "glabs/graphics/ogl_framebuffer.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace glabs
{
	void OglFramebuffer::BindDefaultToPipeline()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OglFramebuffer::OglFramebuffer()
	{
		QueryParamsFromDefaultNativeFramebuffer();
	}

	OglFramebuffer::OglFramebuffer(Params params)
		: mParams(std::move(params))
	{
		CreateNativeFramebuffer();
		AssignNativeFramebufferAttachments();
	}

	OglFramebuffer::OglFramebuffer(OglFramebuffer&& other) noexcept
		: mParams(std::move(other.mParams))
		, mNativeFramebuffer(std::exchange(other.mNativeFramebuffer, 0))
		, mColorAttachment(std::move(other.mColorAttachment))
		, mDepthAttachment(std::move(other.mDepthAttachment))
	{
		other.QueryParamsFromDefaultNativeFramebuffer();
	}

	OglFramebuffer& OglFramebuffer::operator=(OglFramebuffer&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		DestroyNativeFramebuffer();
		mParams = std::move(other.mParams);
		mNativeFramebuffer = std::exchange(other.mNativeFramebuffer, 0);
		mColorAttachment = std::move(other.mColorAttachment);
		mDepthAttachment = std::move(other.mDepthAttachment);
		other.QueryParamsFromDefaultNativeFramebuffer();

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

	const OglTexture2D& OglFramebuffer::GetColorAttachment() const
	{
		return mColorAttachment;
	}

	const OglTexture2D& OglFramebuffer::GetDepthAttachment() const
	{
		return mDepthAttachment;
	}

	void OglFramebuffer::BindToPipeline()
	{
		// TODO: should we explicitly provide target instead of hardcoding it?
		glBindFramebuffer(GL_FRAMEBUFFER, mNativeFramebuffer);
	}

	void OglFramebuffer::ClearColor(glm::vec4 color)
	{
		glClearNamedFramebufferfv(mNativeFramebuffer, GL_COLOR, 0, glm::value_ptr(color));
	}

	void OglFramebuffer::ClearDepth(float depth)
	{
		glClearNamedFramebufferfv(mNativeFramebuffer, GL_DEPTH, 0, &depth);
	}

	void OglFramebuffer::QueryParamsFromDefaultNativeFramebuffer()
	{
		// FIXME: probably not going to play well with default framebuffer...

		mColorAttachment = OglTexture2D();
		mDepthAttachment = OglTexture2D();
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

	void OglFramebuffer::AssignNativeFramebufferAttachments()
	{
		mColorAttachment = OglTexture2D(mParams.ColorAttachmentParams);
		mDepthAttachment = OglTexture2D(mParams.DepthAttachmentParams);

		glNamedFramebufferTexture(
			mNativeFramebuffer,
			GL_COLOR_ATTACHMENT0,
			mColorAttachment.GetNativeTexture2D(),
			mParams.ColorAttachmentMipMapIndex
		);

		glNamedFramebufferTexture(
			mNativeFramebuffer,
			GL_DEPTH_ATTACHMENT,
			mDepthAttachment.GetNativeTexture2D(),
			mParams.DepthAttachmentMipMapIndex
		);
	}
}

