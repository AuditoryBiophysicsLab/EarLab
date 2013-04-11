#ifndef __LOGGER_H_INCLUDED
#define __LOGGER_H_INCLUDED

#ifndef WIN32
#define __stdcall
#endif

typedef void(__stdcall *LogCallback)(char*);

class Logger
{
public:
	Logger();
	void SetLogCallback(LogCallback theCallback);
	void Log(const char *Message, ...);
private:
	void __stdcall DefaultCallback(char *Message);
	LogCallback mCallback;
	char mLogBuffer[1024];
};

#ifdef DEFINE_GLOBAL_LOGGER
Logger gLogger;
#else
extern Logger gLogger;
#endif

#endif
