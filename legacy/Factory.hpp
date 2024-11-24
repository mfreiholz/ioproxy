#pragma once
#include <ioproxy/Engine.hpp>

namespace ioproxy
{
	class Factory
	{
	public:
		Factory() = delete;

		/**
			Creates new objects based on "ioConfig".
			This is the central factory function to create all known IOBase implementations,
			based on a generic config describtion.
		**/
		static std::unique_ptr<IOBase> createIO(const Config::IO& config);
	};
}