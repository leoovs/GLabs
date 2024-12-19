#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	enum class FramebufferAttachment
	{
		Color0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Color6,
		Color7,
		Depth,

		Count_,
	};

	constexpr GLuint FramebufferAttachmentToNative(FramebufferAttachment attachment)
	{
		switch (attachment)
		{
		case FramebufferAttachment::Color0:
			return GL_COLOR_ATTACHMENT0;
		case FramebufferAttachment::Color1:
			return GL_COLOR_ATTACHMENT1;
		case FramebufferAttachment::Color2:
			return GL_COLOR_ATTACHMENT2;
		case FramebufferAttachment::Color3:
			return GL_COLOR_ATTACHMENT3;
		case FramebufferAttachment::Color4:
			return GL_COLOR_ATTACHMENT4;
		case FramebufferAttachment::Color5:
			return GL_COLOR_ATTACHMENT5;
		case FramebufferAttachment::Color6:
			return GL_COLOR_ATTACHMENT6;
		case FramebufferAttachment::Color7:
			return GL_COLOR_ATTACHMENT7;
		case FramebufferAttachment::Depth:
			return GL_DEPTH_ATTACHMENT;
		default:
			return 0;
		}

		return 0;
	}
}

