#include "glabs/graphics/ogl_framebuffer.hpp"
#include "glabs/pch.hpp"

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
		, mAttachments(std::move(other.mAttachments))
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
		mAttachments = std::move(other.mAttachments);

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

	void OglFramebuffer::BindToPipeline()
	{
		assert(0 != mNativeFramebuffer);

		// TODO: should we explicitly provide target instead of hardcoding it?
		glBindFramebuffer(GL_FRAMEBUFFER, mNativeFramebuffer);

		GLenum drawBuffers[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glNamedFramebufferDrawBuffers(mNativeFramebuffer, std::size(drawBuffers), drawBuffers);
	}

	void OglFramebuffer::ClearColor(glm::vec4 color, size_t index)
	{
		assert(0 != mNativeFramebuffer);
		glClearNamedFramebufferfv(mNativeFramebuffer, GL_COLOR, GLint(index), glm::value_ptr(color));
	}

	void OglFramebuffer::ClearDepth(float depth)
	{
		assert(0 != mNativeFramebuffer);
		glClearNamedFramebufferfv(mNativeFramebuffer, GL_DEPTH, 0, &depth);
	}

	void OglFramebuffer::AttachTexture2D(
		FramebufferAttachment attachment,
		const OglTexture2D& texture,
		int32_t arrayIndex,
		int32_t mipLevelIndex
	)
	{
		mAttachments.at(size_t(attachment)) = std::make_unique<
			OglFramebufferTexture2DBinding
		>(*this, texture, attachment, arrayIndex, mipLevelIndex);
	}

	void OglFramebuffer::AttachCubemapFace(
		FramebufferAttachment attachment,
		const OglCubemap& cubemap,
		CubemapFace face,
		int32_t arrayIndex,
		int32_t mipLevelIndex
	)
	{
		mAttachments.at(size_t(attachment)) = std::make_unique<
			OglFramebufferCubemapBinding
		>(*this, cubemap, attachment, face, arrayIndex, mipLevelIndex);
	}

	void OglFramebuffer::BindAttachment(FramebufferAttachment attachment)
	{
		OglFramebufferBinding* binding = mAttachments.at(size_t(attachment)).get();
		if (binding)
		{
			binding->Bind();
		}
	}

	void OglFramebuffer::UnbindAttachment(FramebufferAttachment attachment)
	{
		OglFramebufferBinding* binding = mAttachments.at(size_t(attachment)).get();
		if (binding)
		{
			binding->Unbind();
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

