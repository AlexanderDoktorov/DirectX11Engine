#include "Logger.h"


Logger::Logger(const std::string& logFileName, const std::string& errFileName)
	: 
	logFile(logFileName),
	errFile(errFileName)
{
}

Logger::~Logger()
{
	logFile.close();
	errFile.close();
}

void Logger::RedirectErrors(std::ostream* oStream) const noexcept
{
	RedirectStreamImpl(&errFile, oStream);
}

void Logger::RedirectLog(std::ostream* oStream) const noexcept
{
	RedirectStreamImpl(&logFile, oStream);
}

void Logger::LogError(const std::ofstream::char_type* message)
{
	errFile << message;
}

void Logger::LogMsg(const std::ofstream::char_type* message)
{
	logFile << message;
}

