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
			emit errorOccured(m_file.errorString());
			return;
		}
		emit ready();
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
			HL_ERROR(LL, m_file.errorString().toStdString());
			return;
		}
		m_file.write(data);
		if (m_options.immediate)
			m_file.flush();
	}

private:
	Options m_options;
	QFile m_file;
};