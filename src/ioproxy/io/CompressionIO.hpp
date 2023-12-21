#pragma once
#include "../IOBase.hpp"

namespace ioproxy
{
	/*
		Provides ZLIB compression.
	*/
	class CompressionIO : public IOBase
	{
		Q_OBJECT

	public:
		struct Options
		{
			/*
				c = compress
				u = uncompress
			*/
			char mode = 'c';

			// 0-9
			int level = -1;
		};

		static const QString TYPE;

		CompressionIO() = default;
		~CompressionIO() override = default;

		CompressionIO(const CompressionIO&) = delete;
		CompressionIO& operator=(const CompressionIO&) = delete;

		void setOptions(const Options& options)
		{
			m_options = options;
		}

		void start() override
		{
			emit started();
		}

		void stop() override
		{
		}

		void writeData(const DataPack& data) override
		{
			DataPack pack(qCompress(data.bytes, m_options.level));
			pack.parameters.insert("source_size", data.bytes.length());
			emit newData(pack);
		}

	private:
		Options m_options;
	};


}