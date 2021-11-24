#pragma once
#include "../IOBase.hpp"
#include <memory>

class WebSocketClientIO : public IOBase
{
	Q_OBJECT

public:
	WebSocketClientIO();
	~WebSocketClientIO() override;
	void start() override;
	void stop() override;
	void writeData(const DataPack& data) override;

private:
	class P;
	std::unique_ptr<P> d;
};