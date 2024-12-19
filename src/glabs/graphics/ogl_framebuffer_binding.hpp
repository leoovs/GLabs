#pragma once

#include "glabs/graphics/cubemap_face.hpp"
#include "glabs/graphics/framebuffer_attachment.hpp"
#include "glabs/graphics/ogl_cubemap.hpp"
#include "glabs/pch.hpp"

namespace glabs
{
	class OglFramebuffer;
	class OglTexture2D;

	class OglFramebufferBinding
	{
	public:
		enum class Kind
		{
			Texture2D,
			CubemapFace,
		};

		virtual ~OglFramebufferBinding() = default;

		virtual Kind GetKind() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};

	class OglFramebufferTexture2DBinding final : public OglFramebufferBinding
	{
	public:
		OglFramebufferTexture2DBinding(
			const OglFramebuffer& framebuffer,
			const OglTexture2D& texture,
			FramebufferAttachment attachment,
			int32_t arrayIndex = 0,
			int32_t mipIndex = 0
		);
		~OglFramebufferTexture2DBinding() override = default;

		Kind GetKind() const override;

		void Bind() override;
		void Unbind() override;

	private:
		const OglFramebuffer* mParentFramebuffer = nullptr;
		const OglTexture2D* mBoundTexture = nullptr;
		FramebufferAttachment mFramebufferAttachment = FramebufferAttachment::Color0;
		int32_t mTextureArrayIndex = 0;
		int32_t mTextureMipLevelIndex = 0;
	};

	class OglFramebufferCubemapBinding final : public OglFramebufferBinding
	{
	public:
		OglFramebufferCubemapBinding(
			const OglFramebuffer& nativeFramebuffer,
			const OglCubemap& cubemap,
			FramebufferAttachment attachment,
			CubemapFace face,
			int32_t arrayIndex = 0,
			int32_t mipIndex = 0
		);
		~OglFramebufferCubemapBinding() override = default;

		Kind GetKind() const override;

		void Bind() override;
		void Unbind() override;

	private:
		const OglFramebuffer* mParentFramebuffer = nullptr;
		const OglCubemap* mBoundCubemap = nullptr;
		FramebufferAttachment mFramebufferAttachment = FramebufferAttachment::Color0;
		int32_t mCubemapArrayIndex = 0;
		CubemapFace mCubemapFace = CubemapFace::Last_;
		int32_t mCubemapFaceMipLevelIndex = 0;
	};
}

