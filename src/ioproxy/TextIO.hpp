#pragma once
#include "IOBase.hpp"
#include <QDateTime>
#include <QTimer>
#include <chrono>
#include <cinttypes>

/*!
	TextIO produces text based on timing/interval.
*/
class TextIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		QString text = QString("HELLO\n");
		int intervalMS = 1000;
		bool withTime = false;
		bool withCounter = false;
		bool lineBreak = false;
	};

	TextIO()
		: IOBase()
	{}

	~TextIO() override
	{
		stop();
	}

	void setOptions(const Options& options)
	{
		m_options = options;
	}

	void start() override
	{
		m_counter = 0;
		QObject::connect(&m_timer, &QTimer::timeout, this, &TextIO::onTimer);
		m_timer.setInterval(std::chrono::milliseconds(500));
		m_timer.start();
		emit ready();
	}

	void stop() override
	{
		m_timer.disconnect(this);
		m_timer.stop();
	}

private slots:
	void onTimer()
	{
		++m_counter;

		QString str;
		if (m_options.withCounter)
		{
			str.append(QString("%1").arg(m_counter, 5, 10, QChar('0')));
			str.append(" ");
		}
		if (m_options.withTime)
		{
			str.append(QDateTime::currentDateTimeUtc().toString());
			str.append(" ");
		}
		if (!m_options.text.isEmpty())
		{
			str.append(m_options.text);
		}
		if (m_options.lineBreak)
		{
			str.append('\n');
		}
		emit newData(QByteArray::fromStdString(str.toStdString()));
	}

private:
	Options m_options;
	QTimer m_timer;
	int m_counter = 0;
};