#pragma once
#include "../IOBase.hpp"

namespace ioproxy
{
	class CompressionIO : public IOBase
	{
	public:
		static const QString TYPE;
		static const char COMPRESS = 'c';
		static const char UNCOMPRESS = 'u';

		struct Options
		{
			char mode = COMPRESS;
			int level = -1; ///< -1=zlib default; 0-9
		};

		CompressionIO()
			: IOBase()
		{}

		~CompressionIO() override = default;

		CompressionIO(const CompressionIO&) = delete;
		CompressionIO& operator=(const CompressionIO&) = delete;

		void setOptions(const Options& options)
		{
			m_options = options;
		}

		/**
			Besides from validating Options, nothing else required here.
		**/
		void start() override
		{
			if (m_options.mode != COMPRESS && m_options.mode != UNCOMPRESS)
			{
				emit startupErrorOccured(QString("Unknown mode: %1").arg(m_options.mode));
				return;
			}
			if (m_options.level < -1 || m_options.level > 10)
			{
				emit startupErrorOccured(QString("Unsupported compression level: %1").arg(m_options.level));
				return;
			}
			emit started();
		}

		void stop() override
		{}

		void writeData(const DataPack& data) override
		{
			if (m_options.mode == COMPRESS)
			{
				DataPack d(data);
				d.bytes = qCompress(data.bytes, m_options.level);
				emit newData(d);
			}
			else if (m_options.mode = UNCOMPRESS)
			{
				DataPack d(data);
				d.bytes = qUncompress(data.bytes);
				emit newData(d);
			}
		}

	private:
		Options m_options;
	};
}