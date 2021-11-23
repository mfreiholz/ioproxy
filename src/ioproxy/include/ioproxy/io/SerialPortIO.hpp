#pragma once
#include "../Global.hpp"
#include "../IOBase.hpp"
#include <QSerialPort>
#include <QSerialPortInfo>

namespace ioproxy
{
	class SerialPortIO : public IOBase
	{
		Q_OBJECT

	public:
		struct Options
		{
			QString portID;
			int baudrate = QSerialPort::Baud115200;
			QSerialPort::DataBits databits = QSerialPort::Data8;
			QSerialPort::Parity parity = QSerialPort::NoParity;
			QSerialPort::StopBits stopbits = QSerialPort::OneStop;
			QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
			bool breakEnabled = false;
			bool dtr = false;
			bool rts = false;
		};

		static const QString TYPE;

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
			if (!m_port.setBaudRate(m_options.baudrate, QSerialPort::AllDirections))
			{
				emit startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
				return;
			}
			if (!m_port.setBreakEnabled(m_options.breakEnabled))
			{
				emit startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
				return;
			}
			if (!m_port.setDataBits(m_options.databits))
			{
				emit startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
				return;
			}
			if (!m_port.setDataTerminalReady(m_options.dtr))
			{
				emit startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
				return;
			}
			if (!m_port.setFlowControl(m_options.flowControl))
			{
				emit startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
				return;
			}
			if (!m_port.setParity(m_options.parity))
			{
				emit startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
				return;
			}
			if (!m_port.setStopBits(m_options.stopbits))
			{
				emit startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
				return;
			}
			if (!m_port.setRequestToSend(m_options.rts))
			{
				emit startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
				return;
			}
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

		void writeData(const DataPack& data) override
		{
			if (!m_port.isOpen())
			{
				return;
			}
			auto written = m_port.write(data.bytes);
			if (written != data.bytes.size())
			{
				emit errorOccured(QString("writtenBytes (%1) != data.size (%2); port=%3")
									  .arg(written)
									  .arg(data.bytes.size())
									  .arg(m_port.portName()));
				return;
			}

			m_statistic.bytesWritten += written;
		}

	private slots:
		void onReadyRead()
		{
			while (m_port.bytesAvailable())
			{
				auto data = m_port.read(m_port.bytesAvailable());
				m_statistic.bytesRead += data.size();
				emit newData(data);
			}
		}

	private:
		Options m_options;
		QSerialPort m_port;
	};
}