#pragma once

namespace glabs
{
	class AppConfigurator;

	class AppConfig
	{
	public:
		virtual ~AppConfig() = default;

		virtual void Apply(AppConfigurator& configurator) = 0;
		virtual void Undo(AppConfigurator& configurator) = 0;
	};
}

