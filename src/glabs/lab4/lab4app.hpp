#pragma once

#include "glabs/app/basic_app.hpp"

namespace glabs
{
	class Lab4App : public BasicApp
	{
	private:
		std::string_view GetName() const override
		{
			return "lab4";
		}
	};
}

