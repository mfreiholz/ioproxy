#pragma once
#include "../IOBase.hpp"
#include <chrono>

class ByteGeneratorIO : public IOBase
{
	Q_OBJECT

public:
	struct Options
	{
		// Number of bytes to generate per second.
		qint64 bytesPerSecond = 1024 * 1024 * 5;

		// Max data-pack size. Maximum number of bytes to emit
		// with "newData()" with one call.
		qint64 packetSize = 4096;
	};

	ByteGeneratorIO(const Options& options, QObject* parent = nullptr);
	~ByteGeneratorIO() override = default;
	ByteGeneratorIO(const ByteGeneratorIO&) = delete;
	ByteGeneratorIO& operator=(const ByteGeneratorIO&) = delete;

	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;

protected Q_SLOTS:
	void next();

private:
	inline static const std::chrono::nanoseconds ONE_SECOND = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1));

	Options m_options;
	bool m_run = false;
	QByteArray m_packet;

	qint64 m_numberOfPacketsPerSecond = 0;
	qint64 m_numberOfPacketsPerNextCall = 0;

	std::chrono::steady_clock::time_point m_last;
};