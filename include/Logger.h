#pragma once
#include <iostream>
#include <fstream>

struct ILogger
{
	virtual void RedirectErrors(std::ostream* oStream) const noexcept = 0;
	virtual void RedirectLog(std::ostream* oStream) const noexcept = 0;
	virtual void LogError(const std::ofstream::char_type* message) = 0;
	virtual void LogMsg(const std::ofstream::char_type* message) = 0;
};

class Logger : public ILogger
{
public:
	Logger(const std::string& logFileName, const std::string& errFileName);
	~Logger();

	template<class oStream1, class oStream2>
	static void RedirectStreamImpl(oStream1* oStreamOld, oStream2* oStreamNew) noexcept
	{
		oStreamNew->rdbuf(oStreamOld->rdbuf());
	}

	void RedirectErrors(std::ostream* oStream) const noexcept	override;
	void RedirectLog(std::ostream* oStream) const noexcept		override;
	void LogError(const std::ofstream::char_type* message)		override;
	void LogMsg(const std::ofstream::char_type* message)		override;
private:
	std::ofstream logFile;
	std::ofstream errFile;
};

class EmptyLogger : public ILogger
{
public:
	EmptyLogger() = default;
	virtual void RedirectErrors(std::ostream* oStream) const noexcept	{ return; }
	virtual void RedirectLog(std::ostream* oStream) const noexcept		{ return; }
	virtual void LogError(const std::ofstream::char_type* message)		{ return; }
	virtual void LogMsg(const std::ofstream::char_type* message)		{ return; }
};

#ifdef _LOGGING_EN
	inline Logger logger("OutLog.txt", "ErrorLog.txt");
#else
	inline EmptyLogger logger{};
#endif // _LOGGING_EN

