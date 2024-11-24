#pragma once
#include "../Global.hpp"
#include "IOBase.hpp"
#include "IOFactoryBase.hpp"
#include <QSerialPort>
#include <QSerialPortInfo>

///////////////////////////////////////////////////////////////////////
// IO
///////////////////////////////////////////////////////////////////////

/*! ... */
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

	SerialPortIO(const Options& options)
		: IOBase()
		, m_options(options)
	{
		QObject::connect(&m_port, &QSerialPort::readyRead, this, &SerialPortIO::onReadyRead);
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
			Q_EMIT startupErrorOccured(QString("Can not find port: %1").arg(m_options.portID));
			return;
			// TODO think about fallback to "port.setNAme(m_options.portID)"
		}

		// open port
		m_port.setPort(selectedPortInfo);
		if (!m_port.open(QIODevice::ReadWrite))
		{
			Q_EMIT startupErrorOccured(QString("Can not open serial port; %1; %2").arg(selectedPortInfo.portName()).arg(m_port.errorString()));
			return;
		}
		if (!m_port.setBaudRate(m_options.baudrate, QSerialPort::AllDirections))
		{
			Q_EMIT startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
			return;
		}
		if (!m_port.setBreakEnabled(m_options.breakEnabled))
		{
			Q_EMIT startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
			return;
		}
		if (!m_port.setDataBits(m_options.databits))
		{
			Q_EMIT startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
			return;
		}
		if (!m_port.setDataTerminalReady(m_options.dtr))
		{
			Q_EMIT startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
			return;
		}
		if (!m_port.setFlowControl(m_options.flowControl))
		{
			Q_EMIT startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
			return;
		}
		if (!m_port.setParity(m_options.parity))
		{
			Q_EMIT startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
			return;
		}
		if (!m_port.setStopBits(m_options.stopbits))
		{
			Q_EMIT startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
			return;
		}
		if (!m_port.setRequestToSend(m_options.rts))
		{
			Q_EMIT startupErrorOccured(QString("%1 (code=%2)").arg(m_port.errorString()).arg(m_port.error()));
			return;
		}
		Q_EMIT started();
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

		// Fix: Same as in TcpSocketIO, we wait until the bytes have been written.
		m_port.waitForBytesWritten(-1);

		m_statistic.bytesWritten += written;
	}

private Q_SLOTS:
	void onReadyRead()
	{
		while (m_port.bytesAvailable())
		{
			auto data = m_port.read(m_port.bytesAvailable());
			m_statistic.bytesRead += data.size();
			Q_EMIT newData(data);
		}
	}

private:
	Options m_options;
	QSerialPort m_port;
};


///////////////////////////////////////////////////////////////////////
// Factory
///////////////////////////////////////////////////////////////////////

class SerialPortIOFactory : public BuiltInIOFactoryBase
{
public:
	inline static const QString ID = QString("serialport");

	SerialPortIOFactory()
		: BuiltInIOFactoryBase()
	{
		IOParaDef def;

		def.key = "port";
		def.description = "Name of the serial port. Windows: COM1, Linux: /dev/ttyS0";
		def.valueDataType = IOParaDef::STRING;
		def.required = true;
		def.multi = false;
		def.defaultValue = QVariant();
		addParaDef(def);

		def.key = "baudrate";
		def.description = "Baudrate to use for this port.";
		def.valueDataType = IOParaDef::UINT32;
		def.required = false;
		def.multi = false;
		def.defaultValue = 115200;
		addParaDef(def);

		def.key = "databits";
		def.description = "The number of data bits used.";
		def.valueDataType = IOParaDef::UINT8;
		def.required = false;
		def.multi = false;
		def.defaultValue = 8;
		addParaDef(def);

		def.key = "parity";
		def.description = "Parity scheme. 0 = NoParity, 2 = EvenParity, 3 = OddParity, 4 = SpaceParity, 5 = MarkParity";
		def.valueDataType = IOParaDef::UINT8;
		def.required = false;
		def.multi = false;
		def.defaultValue = 0;
		addParaDef(def);

		def.key = "stopbits";
		def.description = "The number of stop bits used. 1 = OneStop; 2 = TwoStop; 3 = OneAndHalfStop; -1 = UnknownStopBits";
		def.valueDataType = IOParaDef::UINT8;
		def.required = false;
		def.multi = false;
		def.defaultValue = 1;
		addParaDef(def);

		def.key = "flow";
		def.description = "0 = Disabled, 1 = Hardware flow control (RTS/CTS), 2 = Software flow control (XON/XOFF)";
		def.valueDataType = IOParaDef::UINT8;
		def.required = false;
		def.multi = false;
		def.defaultValue = 0;
		addParaDef(def);
	}

	QString getID() const override
	{
		return ID;
	}

	tl::expected<IOBase*, QString> create(const IOParameters& params)
	{
		auto eParaCheck = checkRequiredParameters(params);
		if (!eParaCheck)
			return tl::make_unexpected(eParaCheck.error());

		SerialPortIO::Options opts;
		opts.portID = getParaValue(params, m_paraDefsByKey["port"]).value().toString();
		opts.baudrate = getParaValue(params, m_paraDefsByKey["baudrate"]).value().toUInt();
		opts.databits = static_cast<QSerialPort::DataBits>(getParaValue(params, m_paraDefsByKey["databits"]).value().toUInt());
		opts.parity = static_cast<QSerialPort::Parity>(getParaValue(params, m_paraDefsByKey["parity"]).value().toUInt());
		opts.stopbits = static_cast<QSerialPort::StopBits>(getParaValue(params, m_paraDefsByKey["stopbits"]).value().toUInt());
		opts.flowControl = static_cast<QSerialPort::FlowControl>(getParaValue(params, m_paraDefsByKey["flow"]).value().toUInt());
		return {new SerialPortIO(opts)};
	}
};