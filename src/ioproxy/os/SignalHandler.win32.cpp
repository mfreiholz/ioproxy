#ifdef WIN32
#include "SignalHandler.hpp"
#include <Windows.h>

os::SignalHandler* g_signalHandler(nullptr);

os::Signal native2enum(DWORD native)
{
	switch (native)
	{
		case CTRL_C_EVENT:
			return os::Signal::Int;
		case CTRL_BREAK_EVENT:
			return os::Signal::Term;
		case CTRL_CLOSE_EVENT:
			return os::Signal::Close;
		default:
			return os::Signal::Unhandled;
	}
}

DWORD signal2enum(os::Signal sig)
{
	switch (sig)
	{
		case os::Signal::Int:
			return CTRL_C_EVENT;
		case os::Signal::Term:
			return CTRL_BREAK_EVENT;
		case os::Signal::Close:
			return CTRL_CLOSE_EVENT;
		default:
			return 0;
	}
}

BOOL WINAPI signalHandlerFunc(DWORD nativeSignal)
{
	if (!g_signalHandler)
		return FALSE;

	os::Signal signals = native2enum(nativeSignal);
	return g_signalHandler->handleSignal(signals);
}

namespace os
{
	SignalHandler::SignalHandler()
	{
		g_signalHandler = this;
		SetConsoleCtrlHandler(signalHandlerFunc, TRUE);
	}

	SignalHandler::~SignalHandler()
	{
		SetConsoleCtrlHandler(signalHandlerFunc, FALSE);
	}
}
#endif