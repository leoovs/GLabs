#pragma once

#include "glabs/pch.hpp"
#include "glabs/app/app_config.hpp"

namespace glabs
{
	class AppConfigurator
	{
	public:
		template<typename ConfigT, typename... ArgsT>
		ConfigT& SetConfig(ArgsT&&... args)
		{
			auto config = std::make_unique<ConfigT>(std::forward<ArgsT>(args)...);
			auto configPtr = config.get();
			std::type_index configIndex = typeid(ConfigT);

			mSetOrder.push_back(std::ref(*config));
			mConfigs[configIndex] = std::move(config);

			return *configPtr;
		}

		template<typename ConfigT>
		ConfigT& GetConfig() const
		{
			return static_cast<ConfigT&>(*mConfigs.at(typeid(ConfigT)));
		}

		void Apply();
		void Undo();

	private:
		std::unordered_map<std::type_index, std::unique_ptr<AppConfig>> mConfigs;
		std::vector<std::reference_wrapper<AppConfig>> mSetOrder;
	};
}

