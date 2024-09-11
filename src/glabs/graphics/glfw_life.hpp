#pragma once

namespace glabs
{
	class GlfwLife
	{
	public:
		struct Params
		{
			bool EnableDebugContext = true;
			int OglVersionMajor = 4;
			int OglVersionMinor = 6;
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

