#pragma once

enum class Signals : int
{
	Unhandled = 0,
	Noop = 1,
	Int = 2,
	Term = 4,
	Close = 8,
	Reload = 16,
	Defaults = Int | Term | Close
};

class AbstractSignalHandler
{
public:
	virtual ~AbstractSignalHandler() = default;
	virtual bool handleSignal(Signals signal) = 0;
};