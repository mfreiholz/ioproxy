#include <ioproxy/Factory.hpp>
#include <ioproxy/io/CompressionIO.hpp>
#include <ioproxy/io/FileReaderIO.hpp>
#include <ioproxy/io/FileWriterIO.hpp>
#include <ioproxy/io/SerialPortIO.hpp>
#include <ioproxy/io/StdOutIO.hpp>
#include <ioproxy/io/TcpServerIO.hpp>
#include <ioproxy/io/TcpSocketIO.hpp>
#include <ioproxy/io/UdpSocketIO.hpp>
using namespace ioproxy;

std::unique_ptr<IOBase> Factory::createIO(const Config::IO& config)
{
	std::unique_ptr<IOBase> baseIO;
	const auto pend = config.parameters.cend();

	if (config.type.compare(StdOutIO::TYPE) == 0)
	{
		StdOutIO::Options options = {};

		auto io = std::make_unique<StdOutIO>();
		io->setOptions(options);
		baseIO = std::move(io);
	}
	else if (config.type.compare(FileReaderIO::TYPE) == 0)
	{
		FileReaderIO::Options options;
		options.filePath = config.parameters.value("file", "");
		options.bufferSize = config.parameters.value("buffer", QString::number(options.bufferSize)).toInt();

		auto io = std::make_unique<FileReaderIO>(options);
		baseIO = std::move(io);
	}
	else if (config.type.compare(FileWriterIO::TYPE) == 0)
	{
		FileWriterIO::Options options;
		options.filePath = config.parameters.value("file", "");
		options.append = config.parameters.value("append", "0").toInt() != 0;
		options.immediate = config.parameters.value("immediate", "0").toInt() != 0;

		auto io = std::make_unique<FileWriterIO>();
		io->setOptions(options);
		baseIO = std::move(io);
	}
	else if (config.type.compare(TcpServerIO::TYPE) == 0)
	{
		TcpServerIO::Options options;
		options.bindAddress = QHostAddress(config.parameters.value("bind_address"));
		options.bindPort = config.parameters.value("bind_port").toUShort();
		options.maxConnections = config.parameters.value("max_clients", "1").toUInt();
		options.broadcast = config.parameters.value("broadcast_clients", "0").toUInt() == 1;

		auto io = std::make_unique<TcpServerIO>();
		io->setOptions(options);
		baseIO = std::move(io);
	}
	else if (config.type.compare(TcpSocketIO::TYPE) == 0)
	{
		TcpSocketIO::Options options;
		options.remoteAddress = QHostAddress(config.parameters.value("remote_address"));
		options.remotePort = config.parameters.value("remote_port").toUShort();
		options.reconnect = config.parameters.value("reconnect", "0").toUShort() == 1;
		options.tcpNoDelayOption = config.parameters.value("low_delay", "0").toUShort() == 1;
		options.tcpKeepAliveOption = config.parameters.value("keep_alive", "0").toUShort() == 1;

		auto io = std::make_unique<TcpSocketIO>();
		io->setOptions(options);
		baseIO = std::move(io);
	}
	else if (config.type.compare(UdpSocketIO::TYPE) == 0)
	{
		UdpSocketIO::Options options;
		if (config.parameters.contains("bind_address"))
		{
			options.bindAddress = QHostAddress(config.parameters.value("bind_address"));
		}
		if (config.parameters.contains("bind_port"))
		{
			options.bindPort = config.parameters.value("bind_port").toUShort();
		}
		auto addresses = config.parameters.values("remote_address");
		auto ports = config.parameters.values("remote_port");
		for (int i = 0; i < addresses.size(); ++i)
		{
			QHostAddress addr(addresses[i]);
			uint16_t port = 0;
			if (ports.size() > 0 && i < ports.size())
			{
				port = ports[i].toUShort();
			}
			options.remoteAddresses.push_back(std::make_pair(addr, port));
		}
		if (config.parameters.contains("echo"))
		{
			options.echo = config.parameters.value("echo").toUShort() != 0;
		}
		if (config.parameters.contains("multicast_ttl"))
		{
			options.multicastTTL = config.parameters.value("multicast_ttl").toUShort();
		}
		options.multicastLoopback = config.parameters.value("multicast_loopback", "0").toUShort() != 0;

		auto io = std::make_unique<UdpSocketIO>();
		io->setOptions(options);
		baseIO = std::move(io);
	}
	else if (config.type.compare(SerialPortIO::TYPE) == 0)
	{
		SerialPortIO::Options options;
		options.portID = config.parameters.value("port");
		if (auto it = config.parameters.find("baudrate"); it != config.parameters.end())
		{
			options.baudrate = it.value().toUInt();
		}
		if (auto it = config.parameters.find("databits"); it != config.parameters.end())
		{
			options.databits = static_cast<QSerialPort::DataBits>(it->toInt());
		}
		if (auto it = config.parameters.find("parity"); it != config.parameters.end())
		{
			options.parity = static_cast<QSerialPort::Parity>(it->toInt());
		}
		if (auto it = config.parameters.find("stopbits"); it != config.parameters.end())
		{
			options.stopbits = static_cast<QSerialPort::StopBits>(it->toInt());
		}
		if (auto it = config.parameters.find("flow"); it != config.parameters.end())
		{
			switch (it.value().toUInt())
			{
				case 0:
					options.flowControl = QSerialPort::FlowControl::NoFlowControl;
					break;
				case 1:
					options.flowControl = QSerialPort::FlowControl::HardwareControl;
					break;
				case 2:
					options.flowControl = QSerialPort::FlowControl::SoftwareControl;
					break;
			}
		}
		if (auto it = config.parameters.find("break"); it != config.parameters.end())
		{
			options.breakEnabled = it->toUInt() == 1;
		}
		if (auto it = config.parameters.find("dtr"); it != config.parameters.end())
		{
			options.dtr = it->toUInt() == 1;
		}
		else if (it = config.parameters.find("rts"); it != config.parameters.end())
		{
			options.rts = it->toUInt() == 1;
		}

		auto io = std::make_unique<SerialPortIO>();
		io->setOptions(options);
		baseIO = std::move(io);
	}
	else if (config.type.compare(CompressionIO::TYPE) == 0)
	{
		CompressionIO::Options options;
		options.mode = config.parameters.value("mode", "c").at(0).toLatin1();
		options.level = config.parameters.value("level", "-1").toInt();

		auto io = std::make_unique<CompressionIO>();
		io->setOptions(options);
		baseIO = std::move(io);
	}
	if (baseIO)
		baseIO->setUniqueName(config.id);
	return baseIO;
}