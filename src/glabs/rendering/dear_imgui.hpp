#pragma once

#include "glabs/graphics/window.hpp"

namespace glabs
{
	class DearImGui
	{
	public:
		struct Params
		{
			Window* OutputWindow = nullptr;
		};

		DearImGui() = default;
		DearImGui(Params params);

		DearImGui(DearImGui&& other) noexcept;

		DearImGui& operator=(DearImGui&& other) noexcept;

		~DearImGui();

		const Params& GetParams() const;

		void NewFrame();
		void RenderDrawData(ImDrawData* data);

	private:
		void InitContext();
		void ShutDownContext();

		Params mParams;
		bool mIsContextAcquired = false;
	};
}

