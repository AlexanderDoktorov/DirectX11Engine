#pragma once
#include <exception>
#include <string>

class baseException
{
public:
	baseException(int codeLine, std::string_view codeFileName) noexcept;

	int					GetLine()		const noexcept;
			std::string GetFileName()	const noexcept;
	virtual std::string what()			const;

private:
	int codeLine = -1;
	std::string codeFileName = "Unknown file";
};