#pragma once
#include "../Global.hpp"
#include "../IOBase.hpp"
#include <QFile>
#include <QObject>
#include <QString>

namespace ioproxy
{
	/*
		Writes all incoming data to file.
		@todo Add timer which flushes every second, if no data has arrived.
	*/
	class FileWriterIO : public IOBase
	{
		Q_OBJECT

	public:
		struct Options
		{
			QString filePath;
			bool append = false;
			bool immediate = false;
		};

		static const QString TYPE;

		FileWriterIO()
			: IOBase()
		{}

		~FileWriterIO()
		{
			this->stop();
		}

		void setOptions(const Options& options)
		{
			m_options = options;
		}

		void start() override
		{
			QIODevice::OpenMode m;
			m |= QIODevice::WriteOnly;
			if (m_options.append)
				m |= QIODevice::Append;

			m_file.setFileName(m_options.filePath);
			if (!m_file.open(m))
			{
				emit startupErrorOccured(QString("%1 %2").arg(m_file.errorString()).arg(m_options.filePath));
				return;
			}
			emit started();
		}

		void stop() override
		{
			if (m_file.isOpen())
				m_file.close();
		}

		void writeData(const DataPack& data) override
		{
			if (!m_file.isOpen())
			{
				emit errorOccured(QString("File not open."));
				return;
			}
			auto bytesWritten = m_file.write(data.bytes);
			if (bytesWritten != data.bytes.size())
			{
				emit errorOccured(QString("bytesWritten (%1) != data.size (%2)").arg(bytesWritten).arg(data.bytes.size()));
				return;
			}
			m_file.flush();

			m_statistic.bytesWritten += bytesWritten;

			//if (m_options.immediate && !m_file.flush())
			//{
			//	emit errorOccured(QString("Can not flush (immediate write) to file. %1").arg(m_file.errorString()));
			//	return;
			//}
		}

	private:
		Options m_options;
		QFile m_file;
	};
}