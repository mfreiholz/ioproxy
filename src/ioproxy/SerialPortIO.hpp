#pragma once
#include "Global.hpp"
#include "IOBase.hpp"
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialPortIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		QString portID;
		int baudrate = QSerialPort::Baud115200;
		int databits = QSerialPort::Data8;
		int parity = QSerialPort::NoParity;
		int stopbits = QSerialPort::UnknownStopBits;
		bool flowControl = false;
		bool breakEnabled = false;
		bool dtr = false;
		bool rts = false;
	};

	SerialPortIO()
		: IOBase()
	{
		QObject::connect(&m_port, &QSerialPort::readyRead, this, &SerialPortIO::onReadyRead);
	}

	~SerialPortIO() override
	{}

	void setOptions(const Options& options)
	{
		m_options = options;
	}

	void start() override
	{
		auto ports = QSerialPortInfo::availablePorts();
		QSerialPortInfo selectedPortInfo;
		for (int i = 0; i < ports.size(); ++i)
		{
			if (ports[i].portName() == m_options.portID)
			{
				selectedPortInfo = ports[i];
				break;
			}
		}
		if (selectedPortInfo.isNull())
		{
			emit startupErrorOccured(QString("Can not find port: %1").arg(m_options.portID));
			return;
			// TODO think about fallback to "port.setNAme(m_options.portID)"
		}

		// open port
		m_port.setPort(selectedPortInfo);
		if (!m_port.open(QIODevice::ReadWrite))
		{
			emit startupErrorOccured(QString("Can not open serial port; %1; %2").arg(selectedPortInfo.portName()).arg(m_port.errorString()));
			return;
		}
		m_port.setBaudRate(m_options.baudrate, QSerialPort::AllDirections);
		//m_port.setBreakEnabled(false);
		//m_port.setDataBits(m_options.databits);
		//m_port.setDataTerminalReady(false);
		//m_port.setFlowControl(m_options.flowControl);
		//m_port.setParity(m_options.parity);
		//m_port.setStopBits(m_options.stopbits);
		//m_port.setRequestToSend(false);

		emit started();
	}

	void stop() override
	{
		if (!m_port.isOpen())
		{
			return;
		}
		m_port.close();
	}

	void writeData(const QByteArray& data) override
	{
		m_port.write(data);
	}

private slots:
	void onReadyRead()
	{
		while (m_port.bytesAvailable())
		{
			auto data = m_port.read(m_port.bytesAvailable());
			emit newData(data);
		}
	}

private:
	Options m_options;
	QSerialPort m_port;
};