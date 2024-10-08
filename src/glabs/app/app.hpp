#pragma once

#include "glabs/app/app_configurator.hpp"

namespace glabs
{
	class App
	{
	public:
		virtual ~App() = default;

		virtual void Configure(AppConfigurator& configurator) {}

		virtual void Start() {}
		virtual void Update() {}

		virtual bool IsRunning() const = 0;
	};
}

