#pragma once
#include "Signals.hpp"

/*
	Windows signals:
	CTRL_C_EVENT
	CTRL_BREAK_EVENT
	CTRL_CLOSE_EVENT

	\see https://stackoverflow.com/questions/7581343/how-to-catch-ctrlc-on-windows-and-linux-with-qt
*/
class WindowsSignalHandler : public AbstractSignalHandler
{
public:
	WindowsSignalHandler(Signals signalsMask);
	~WindowsSignalHandler() override;

private:
	Signals m_mask = Signals::Defaults;
};