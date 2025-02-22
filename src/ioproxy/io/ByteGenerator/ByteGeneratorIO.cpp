#include "ByteGeneratorIO.hpp"
#include <QMetaObject>
#include <QTimer>
#include <cmath>

ByteGeneratorIO::ByteGeneratorIO(const Options& options, QObject* parent)
	: IOBase()
	, m_options(options)
{
	// Generate random bytes.
	m_packet.resize(m_options.packetSize);

	m_numberOfPacketsPerSecond = std::ceil((double)m_options.bytesPerSecond / (double)m_options.packetSize);
	m_numberOfPacketsPerNextCall = std::ceil(m_numberOfPacketsPerSecond / 10.0);
}

void ByteGeneratorIO::start()
{
	m_run = true;
	m_last = std::chrono::steady_clock::now();
	next();
}

void ByteGeneratorIO::stop()
{
	m_run = false;
}

void ByteGeneratorIO::writeData(const DataPack& data)
{}

void ByteGeneratorIO::next()
{
	if (!m_run)
		return;

	const auto now = std::chrono::steady_clock::now();
	const auto diff = now - m_last;

	for (qint64 i = 0; i < m_numberOfPacketsPerNextCall; ++i)
		Q_EMIT newData(m_packet);

	m_last = now;
	QTimer::singleShot(std::chrono::milliseconds(100), this, &ByteGeneratorIO::next);
}