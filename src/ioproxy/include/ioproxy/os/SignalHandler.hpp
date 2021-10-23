#pragma once
/*
	Windows signals:
	CTRL_C_EVENT
	CTRL_BREAK_EVENT
	CTRL_CLOSE_EVENT

	\see https://stackoverflow.com/questions/7581343/how-to-catch-ctrlc-on-windows-and-linux-with-qt
*/
namespace os
{
	// Known signals
	// Most common: Int, Term, Close
	enum class Signal : int
	{
		Unhandled,
		Noop,
		Int,
		Term,
		Close,
		Reload
	};

	/*
		@todo description...
		@note There may only be one instance of this class.
	*/
	class SignalHandler
	{
	public:
		SignalHandler();
		virtual ~SignalHandler();

		/*
			This function is being called from system dependend implementation
			whenever a signal from OS is emitted.
		*/
		virtual bool handleSignal(Signal signal) = 0;
	};
}