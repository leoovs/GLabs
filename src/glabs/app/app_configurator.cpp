#include "glabs/app/app_configurator.hpp"

namespace glabs
{
	void AppConfigurator::Apply()
	{
		for (auto it = mSetOrder.begin(); it != mSetOrder.end(); it++)
		{
			it->get().Apply(*this);
		}
	}

	void AppConfigurator::Undo()
	{
		for (auto it = mSetOrder.rbegin(); it != mSetOrder.rend(); it++)
		{
			it->get().Undo(*this);
		}
	}
}

