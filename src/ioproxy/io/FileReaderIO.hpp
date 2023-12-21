#pragma once
#include "../IOBase.hpp"
#include <QtCore/QFile>
#include <QtCore/QThread>
#include <QtCore/QTimer>

namespace ioproxy
{
	/*
		Reads file async.
	*/
	class FileReader : public QObject
	{
		Q_OBJECT

	public:
		FileReader(const QString& filePath, int bufferSize)
			: m_file(filePath)
			, m_bufferSize(bufferSize)
			, m_timer()
		{
			m_timer.setSingleShot(true);
			m_timer.setInterval(0);
			QObject::connect(&m_timer, &QTimer::timeout, this, &FileReader::readNext);
		}

		~FileReader() override
		{
			m_timer.stop();
		}

		Q_SLOT void startReading()
		{
			m_file.open(QIODevice::ReadOnly);
			m_timer.start();
		}

	signals:
		void newData(const QByteArray&);

	private:
		QFile m_file;
		int m_bufferSize;
		QTimer m_timer;

		Q_SLOT void readNext()
		{
			m_timer.stop();

			auto ba = m_file.read(m_bufferSize);
			if (ba.size() <= 0)
			{
				// ERROR or DONE.
				return;
			}

			emit newData(ba);
			m_timer.start();
		}
	};

	/*
	*/
	class FileReaderIO : public IOBase
	{
		Q_OBJECT

	public:
		struct Options
		{
			QString filePath;
			int bufferSize = 4096;
		};

		static const QString TYPE;

		FileReaderIO(const Options& options)
			: m_options(options)
			, m_reader(options.filePath, options.bufferSize)
		{
			QObject::connect(&m_reader, &FileReader::newData, [&](const QByteArray& ba) {
				DataPack pack(ba);
				m_statistic.bytesRead += ba.size();
				emit newData(pack);
			});
		}

		void start() override
		{
			m_reader.startReading();
			emit started();
		}

		void stop() override
		{}

	private:
		Options m_options;
		FileReader m_reader;
	};
}