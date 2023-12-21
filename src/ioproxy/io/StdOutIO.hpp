#pragma once
#include "../IOBase.hpp"
#include <iostream>

namespace ioproxy
{
	class StdOutIO : public IOBase
	{
		Q_OBJECT

	public:
		struct Options
		{
		};

		static const QString TYPE;

		StdOutIO();
		~StdOutIO() override;

		StdOutIO(const StdOutIO&) = delete;
		StdOutIO& operator=(const StdOutIO&) = delete;

		void setOptions(const Options& options);
		void start() override;
		void stop() override;
		void writeData(const DataPack& data) override;

	private:
		Options m_options;
	};
}