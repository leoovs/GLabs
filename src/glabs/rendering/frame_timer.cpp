#include "glabs/rendering/frame_timer.hpp"
#include <chrono>

namespace glabs
{
	void FrameTimer::Tick()
	{
		auto now = std::chrono::high_resolution_clock::now();

		mDeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - mPrev).count() * 1e-9f;

		mPrev = std::chrono::high_resolution_clock::now();
	}

	float FrameTimer::GetDeltaTime() const
	{
		return mDeltaTime;
	}
}

