#pragma once

namespace glabs
{
	class GlfwLife
	{
	public:
		struct Params
		{
			bool EnableDebugContext = true;
			std::pair<int32_t, int32_t> Version;
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

