#pragma once
#include "Global.hpp"
#include "IOBase.hpp"
#include <QFile>
#include <QObject>
#include <QString>

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

	void writeData(const QByteArray& data) override
	{
		if (!m_file.isOpen())
		{
			emit errorOccured(QString("File not open."));
			return;
		}
		auto bytesWritten = m_file.write(data);
		if (bytesWritten != data.size())
		{
			emit errorOccured(QString("bytesWritten (%1) != data.size (%2)").arg(bytesWritten).arg(data.size()));
		}
		if (m_options.immediate && !m_file.flush())
		{
			emit errorOccured(QString("Can not flush (immediate write) to file. %1").arg(m_file.errorString()));
		}
	}

private:
	Options m_options;
	QFile m_file;
};