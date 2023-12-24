#pragma once
#include <exception>
#include <string>

class baseException
{
public:
	baseException(int codeLine, const char* codeFileName) noexcept
		: 
		codeLine(codeLine),
		codeFileName(codeFileName)
	{

	}

	int GetLine() const noexcept
	{
		return codeLine;
	}

	std::string GetFileName() const noexcept
	{
		return codeFileName;
	}

	virtual std::string what() const
	{
		using namespace std::string_literals;
		return "[File] "s + codeFileName + " [Line] "s + std::to_string(codeLine);
	}

private:
	int codeLine = -1;
	std::string codeFileName = "Unknown file";
};