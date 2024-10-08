#pragma once

#include "glabs/pch.hpp"
#include "glabs/app/app.hpp"
#include "glabs/app/app_configurator.hpp"

namespace glabs
{
	class AppContainer
	{
	public:
		template<typename AppT, typename... ArgsT>
		AppContainer& Bind(ArgsT&&... args)
		{
			assert(!mApp && "App is already bound");
			mApp = std::make_unique<AppT>(std::forward<ArgsT>(args)...);

			return *this;
		}

		AppContainer& Configure();
		void Run();

	private:
		std::unique_ptr<App> mApp;
		AppConfigurator mConfigurator;
	};
}

