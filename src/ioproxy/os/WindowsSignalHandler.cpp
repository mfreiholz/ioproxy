#include "WindowsSignalHandler.hpp"
#include <Windows.h>
#include <set>

// There can only be one global signal handler.
WindowsSignalHandler* g_SignalHandler(NULL);

// Signals to handle.
std::set<int> g_registry;

Signals native2enum(DWORD native)
{
	switch (native)
	{
		case CTRL_C_EVENT:
			return Signals::Int;
		case CTRL_BREAK_EVENT:
			return Signals::Term;
		case CTRL_CLOSE_EVENT:
			return Signals::Close;
		default:
			return Signals::Unhandled;
	}
}

DWORD signal2enum(Signals sig)
{
	switch (sig)
	{
		case Signals::Int:
			return CTRL_C_EVENT;
		case Signals::Term:
			return CTRL_BREAK_EVENT;
		case Signals::Close:
			return CTRL_CLOSE_EVENT;
		default:
			return 0;
	}
}

BOOL WINAPI signalHandlerFunc(DWORD nativeSignal)
{
	if (!g_SignalHandler)
		return FALSE;

	Signals signals = native2enum(nativeSignal);
	return g_SignalHandler->handleSignal(signals);
}

WindowsSignalHandler::WindowsSignalHandler(Signals signalsMask)
	: m_mask(signalsMask)
{
	g_SignalHandler = this;
	SetConsoleCtrlHandler(signalHandlerFunc, TRUE);
}

WindowsSignalHandler::~WindowsSignalHandler()
{
	SetConsoleCtrlHandler(signalHandlerFunc, FALSE);
}