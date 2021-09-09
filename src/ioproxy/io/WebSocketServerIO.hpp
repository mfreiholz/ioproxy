#pragma once
#include "../IOBase.hpp"
#include <memory>

class WebSocketServerIO : public IOBase
{
	Q_OBJECT

public:
	WebSocketServerIO();
	~WebSocketServerIO() override;
	void start() override;
	void stop() override;
	void writeData(const QByteArray& data) override;

private:
	class P;
	std::unique_ptr<P> d;
};