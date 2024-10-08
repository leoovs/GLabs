#include "glabs/app/app_container.hpp"

namespace glabs
{
	AppContainer& AppContainer::Configure()
	{
		assert(mApp && "App binding is required to configure");
		mApp->Configure(mConfigurator);
		return *this;
	}

	void AppContainer::Run()
	{
		assert(mApp && "App binding is required to run");

		mConfigurator.Apply();
		mApp->Start();

		while (mApp->IsRunning())
		{
			mApp->Update();
		}
		mApp.reset();

		mConfigurator.Undo();
	}
}

