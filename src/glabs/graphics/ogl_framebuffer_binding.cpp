#include "glabs/graphics/ogl_framebuffer_binding.hpp"

#include "glabs/graphics/framebuffer_attachment.hpp"
#include "glabs/graphics/ogl_framebuffer.hpp"
#include "glabs/graphics/ogl_texture2d.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	// Texture2D binding.

	OglFramebufferTexture2DBinding::OglFramebufferTexture2DBinding(
		const OglFramebuffer& framebuffer,
		const OglTexture2D& texture,
		FramebufferAttachment attachment,
		int32_t arrayIndex,
		int32_t mipIndex
	)
		: mParentFramebuffer(&framebuffer)
		, mBoundTexture(&texture)
		, mFramebufferAttachment(attachment)
		, mTextureArrayIndex(arrayIndex)
		, mTextureMipLevelIndex(mipIndex)
	{}

	OglFramebufferBinding::Kind OglFramebufferTexture2DBinding::GetKind() const
	{
		return Kind::Texture2D;
	}

	void OglFramebufferTexture2DBinding::Bind()
	{
		glNamedFramebufferTextureLayer(
			mParentFramebuffer->GetNativeFramebuffer(),
			FramebufferAttachmentToNative(mFramebufferAttachment),
			mBoundTexture->GetNativeTexture2D(),
			mTextureMipLevelIndex,
			mTextureArrayIndex
		);
	}

	void OglFramebufferTexture2DBinding::Unbind()
	{
		glNamedFramebufferTextureLayer(
			mParentFramebuffer->GetNativeFramebuffer(),
			FramebufferAttachmentToNative(mFramebufferAttachment),
			0,
			0,
			0
		);
	}

	// Cubemap binding.

	OglFramebufferCubemapBinding::OglFramebufferCubemapBinding(
		const OglFramebuffer& nativeFramebuffer,
		const OglCubemap& cubemap,
		FramebufferAttachment attachment,
		CubemapFace face,
		int32_t arrayIndex,
		int32_t mipIndex
	)
		: mParentFramebuffer(&nativeFramebuffer)
		, mBoundCubemap(&cubemap)
		, mFramebufferAttachment(attachment)
		, mCubemapArrayIndex(arrayIndex)
		, mCubemapFace(face)
	{}

	OglFramebufferBinding::Kind OglFramebufferCubemapBinding::GetKind() const
	{
		return Kind::CubemapFace;
	}

	void OglFramebufferCubemapBinding::Bind()
	{
		glNamedFramebufferTextureLayer(
			mParentFramebuffer->GetNativeFramebuffer(),
			FramebufferAttachmentToNative(mFramebufferAttachment),
			mBoundCubemap->GetNativeCubemap(),
			mCubemapFaceMipLevelIndex,
			mCubemapArrayIndex * int32_t(CubemapFace::Count_) + CubemapFaceToNative(mCubemapFace)
		);
	}

	void OglFramebufferCubemapBinding::Unbind()
	{
		glNamedFramebufferTextureLayer(
			mParentFramebuffer->GetNativeFramebuffer(),
			FramebufferAttachmentToNative(mFramebufferAttachment),
			0,
			0,
			0
		);
	}
}

