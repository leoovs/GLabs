#pragma once

#include "glabs/pch.hpp"

namespace glabs
{
	class FrameTimer
	{
	public:

		void Tick();
		float GetDeltaTime() const;

	private:
		std::chrono::high_resolution_clock::time_point mPrev;
		float mDeltaTime;
	};
}

