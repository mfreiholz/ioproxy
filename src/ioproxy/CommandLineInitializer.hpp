#pragma once
#include "AppContext.hpp"
#include "IOHandler.hpp"
#include <QMultiMap>
#include <QStringList>
#include <iostream>

#include "FileWriterIO.hpp"
#include "StdOutIO.hpp"
#include "TcpServerIO.hpp"
#include "TcpSocketIO.hpp"
#include "TextIO.hpp"
#include "UdpSocketIO.hpp"

/*
	Command line arguments idea.
*/
class CommandLineInitializer
{
	QStringList m_args;
	int m_index = -1;

public:
	CommandLineInitializer(const QStringList& args)
		: m_args(args)
	{}

	bool init(AppContext& context)
	{
		QString val;
		std::shared_ptr<IOHandler> ioHandler;
		QMultiMap<QString, QString> params;
		QStringList connections;

		auto add2context = [&]() {
			if (ioHandler)
			{
				setOptions(ioHandler, params);
				context.add(ioHandler);
				params.clear();
				ioHandler.reset();
			}
		};

		while (next(val))
		{
			// Create new IO by it's type.
			if (val == "-io")
			{
				add2context();
				if (!next(val))
				{
					std::cerr << "Missing type after --io" << std::endl;
					return false;
				}
				if (val == "text")
				{
					auto io = std::make_shared<TextIO>();
					ioHandler = std::make_shared<IOHandler>(io);
				}
				else if (val == "stdout")
				{
					auto io = std::make_shared<StdOutIO>();
					ioHandler = std::make_shared<IOHandler>(io);
				}
				else if (val == "filewriter")
				{
					auto io = std::make_shared<FileWriterIO>();
					ioHandler = std::make_shared<IOHandler>(io);
				}
				else if (val == "udpsocket")
				{
					auto io = std::make_shared<UdpSocketIO>();
					ioHandler = std::make_shared<IOHandler>(io);
				}
				else if (val == "tcpsocket")
				{
					auto io = std::make_shared<TcpSocketIO>();
					ioHandler = std::make_shared<IOHandler>(io);
				}
				else if (val == "tcpserver")
				{
					auto io = std::make_shared<TcpServerIO>();
					ioHandler = std::make_shared<IOHandler>(io);
				}
				else
				{
					std::cerr << "Unknown type after --io" << std::endl;
					return false;
				}
			}
			else if (val == "-id" && ioHandler)
			{
				if (!next(val))
				{
					std::cerr << "Missing value after --id" << std::endl;
					return false;
				}
				ioHandler->setId(val);
			}
			else if (val == "-p" && ioHandler)
			{
				if (!next(val))
				{
					std::cerr << "Empty value for -p not allowed" << std::endl;
					return false;
				}

				auto pos = val.indexOf("=");
				if (pos == -1)
				{
					params.insert(val, QString("true"));
				}
				else
				{
					params.insert(val.mid(0, pos), val.mid(pos + 1));
				}
			}
			else if (val == "--connect")
			{
				if (!next(val))
				{
					HL_ERROR(LL, "Missing value for --connect");
					return false;
				}
				connections.push_back(val.trimmed());
			}
		}
		add2context();

		// manage connections.
		for (const auto& str : connections)
		{
			QStringList conn = str.split(",", QString::SkipEmptyParts);
			if (conn.size() != 2)
				continue;

			auto src = context.findHandlerByID(conn[0]);
			if (!src)
			{
				HL_ERROR(LL, "Can't find IO for connection: " + conn[0].toStdString());
				return false;
			}

			auto dst = context.findHandlerByID(conn[1]);
			if (!dst)
			{
				HL_ERROR(LL, "Can't find IO for connection: " + conn[1].toStdString());
				return false;
			}
			src->addTarget(dst);
		}

		return true;
	}

protected:
	bool current(QString& value)
	{
		value = m_args[m_index];
	}

	bool hasNext()
	{
		return m_args.size() > (m_index + 1);
	}

	bool next(QString& value)
	{
		if (!hasNext())
		{
			value = QString();
			return false;
		}
		value = m_args[++m_index];
		return true;
	}

	bool peekNext(QString& value)
	{
		if (!hasNext())
		{
			value = QString();
			return false;
		}
		value = m_args[m_index + 1];
		return true;
	}

	void setOptions(std::shared_ptr<IOHandler> h, const QMap<QString, QString>& params)
	{
		if (auto io = std::dynamic_pointer_cast<TextIO>(h->io()); io)
		{
			TextIO::Options options;
			options.text = params.value("text");
			options.intervalMS = params.value("interval", "1000").toInt();
			options.withCounter = params.value("counter", "0").toInt() != 0;
			options.withTime = params.value("datetime", "0").toInt() != 0;
			options.lineBreak = params.value("linebreak", "0").toInt() != 0;
			io->setOptions(options);
		}
		else if (auto io = std::dynamic_pointer_cast<StdOutIO>(h->io()); io)
		{
			StdOutIO::Options options;
			io->setOptions(options);
		}
		else if (auto io = std::dynamic_pointer_cast<FileWriterIO>(h->io()); io)
		{
			FileWriterIO::Options options;
			options.filePath = params.value("file", "");
			options.append = params.value("append", "0").toInt() != 0;
			options.immediate = params.value("immediate", "0").toInt() != 0;
			io->setOptions(options);
		}
		else if (auto io = std::dynamic_pointer_cast<UdpSocketIO>(h->io()); io)
		{
			UdpSocketIO::Options options;
			if (params.contains("bind_address"))
			{
				options.bindAddress = QHostAddress(params.value("bind_address"));
			}
			if (params.contains("bind_port"))
			{
				options.bindPort = params.value("bind_port").toUShort();
			}

			auto addresses = params.values("remote_address");
			auto ports = params.values("remote_port");
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

			io->setOptions(options);
		}
		else if (auto io = std::dynamic_pointer_cast<TcpSocketIO>(h->io()); io)
		{
			TcpSocketIO::Options options;
			options.remoteAddress = QHostAddress(params.value("remote_address"));
			options.remotePort = params.value("remote_port").toUShort();
			options.reconnect = true;
			io->setOptions(options);
		}
		else if (auto io = std::dynamic_pointer_cast<TcpServerIO>(h->io()); io)
		{
			TcpServerIO::Options options;
			options.bindAddress = QHostAddress(params.value("bind_address"));
			options.bindPort = params.value("bind_port").toUShort();
			options.maxConnections = params.value("max_clients", "1").toUInt();
			io->setOptions(options);
		}
	}
};