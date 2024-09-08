#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class OglBuffer
	{
	public:
		struct Params
		{
			std::string DebugName = "Unnamed OglBuffer";

			GLenum Target = 0;
			size_t ElementSize = 8;
			size_t ElementCount = 0;
		};

		OglBuffer(const Params& params);

		OglBuffer(OglBuffer&& other) noexcept;

		OglBuffer& operator=(OglBuffer&& other) noexcept;

		~OglBuffer();

		const Params& GetParams() const;
		size_t GetSize() const;

		void SetData(void* data);

		void BindToPipeline();

	private:
		void CreateNativeBuffer();
		void DestroyNativeBuffer();

		Params mParams;
		GLuint mNativeBuffer = 0;
	};
}

