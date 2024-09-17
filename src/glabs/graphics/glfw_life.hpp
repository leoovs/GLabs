#pragma once

namespace glabs
{
	class GlfwLife
	{
	public:
		struct Params
		{
			bool EnableDebugContext = true;
			int OglVersionMajor = 0;
			int OglVersionMinor = 0;
			bool CoreProfile = true;
		};

		GlfwLife(Params params);

		GlfwLife(const GlfwLife& other) = delete;

		GlfwLife& operator=(const GlfwLife& other) = delete;

		~GlfwLife();

	private:
		void InitGlfw();
		void TerminateGlfw();

		Params mParams;
	};
}

